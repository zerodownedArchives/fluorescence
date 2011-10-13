
#include "ingameobject.hpp"

#include <stdio.h>

#include <misc/log.hpp>

#include <ui/texture.hpp>
#include <ui/manager.hpp>
#include <ui/ingamerenderqueue.hpp>

#include "overheadmessage.hpp"

namespace fluo {
namespace world {

IngameObject::IngameObject(unsigned int objectType) : draggable_(false), objectType_(objectType), visible_(true), renderDataValid_(false), textureProviderUpdateRequired_(true) {
    for (unsigned int i = 0; i < 6; ++i) {
        renderPriority_[i] = 0;
        vertexNormals_[i] = CL_Vec3f(0, 0, 1);
    }
    hueInfo_ = CL_Vec2f(0.0f, 0.0f);
}

IngameObject::~IngameObject() {
}

bool IngameObject::isVisible() const {
    return visible_;
}

void IngameObject::setVisible(bool visible) {
    visible_ = visible;
}

void IngameObject::setLocation(int locX, int locY, int locZ) {
    CL_Vec3<int> oldLocation = location_;

    location_ = CL_Vec3<int>(locX, locY, locZ);

    //LOGARG_DEBUG(LOGTYPE_WORLD, "Object location: %i %i %i", locX, locY, locZ);

    invalidateRenderData();
}

bool IngameObject::isRenderDataValid() const {
    return renderDataValid_;
}

void IngameObject::invalidateRenderData(bool updateTextureProvider) {
    renderDataValid_ = false;

    if (updateTextureProvider) {
        requestUpdateTextureProvider();
    }

    if (!childObjects_.empty()) {
        std::list<boost::shared_ptr<IngameObject> >::iterator iter = childObjects_.begin();
        std::list<boost::shared_ptr<IngameObject> >::iterator end = childObjects_.end();

        for (; iter != end; ++iter) {
            (*iter)->invalidateRenderData();
        }
    }
}

const CL_Vec2f* IngameObject::getVertexCoordinates() const {
    return vertexCoordinates_;
}

const CL_Vec2f& IngameObject::getHueInfo() const {
    return hueInfo_;
}

int IngameObject::getRenderPriority(unsigned int lvl) const {
    if (lvl > 5) {
        lvl = 0;
    }

    return renderPriority_[lvl];
}

const int* IngameObject::getRenderPriorities() const {
    return renderPriority_;
}

bool IngameObject::updateRenderData(unsigned int elapsedMillis) {
    bool bigUpdate = false;
    if (!isRenderDataValid()) {
        bigUpdate = true;
        if (textureProviderUpdateRequired_) {
            updateTextureProvider();
            textureProviderUpdateRequired_ = false;
        }

        boost::shared_ptr<ui::Texture> tex = getIngameTexture();
        if (!tex || !tex->isReadComplete()) {
            return false;
        }

        updateAnimation(elapsedMillis);

        updateVertexCoordinates();
        updateRenderPriority();

        renderDataValid_ = true;
    } else {
        bool frameChanged = updateAnimation(elapsedMillis);

        if (frameChanged) {
            updateVertexCoordinates();
        }
    }

    return bigUpdate;
}

void IngameObject::requestUpdateTextureProvider() {
    textureProviderUpdateRequired_ = true;
}

bool IngameObject::isInDrawArea(int leftPixelCoord, int rightPixelCoord, int topPixelCoord, int bottomPixelCoord) const {
    //LOGARG_DEBUG(LOGTYPE_WORLD, "isInDrawArea (%u %u %u %u) => x=%u y=%u\n", leftPixelCoord, rightPixelCoord, topPixelCoord, bottomPixelCoord, vertexCoordinates_[0u].x, vertexCoordinates_[0u].y);

    // almost every texture is a rectangle, with vertexCoordinates_[0] being top left and vertexCoordinates_[5] bottom right
    // all non-rectangular cases are maptiles
    return vertexCoordinates_[0].x <= rightPixelCoord &&
            vertexCoordinates_[0].y <= bottomPixelCoord &&
            vertexCoordinates_[5].x >= leftPixelCoord &&
            vertexCoordinates_[5].y >= topPixelCoord;
}

bool IngameObject::hasPixel(int pixelX, int pixelY) const {
    // almost every texture is a rectangle, with vertexCoordinates_[0] being top left and vertexCoordinates_[5] bottom right
    // all non-rectangular cases are maptiles
    bool coordinateCheck = vertexCoordinates_[0].x <= pixelX &&
            vertexCoordinates_[0].y <= pixelY &&
            vertexCoordinates_[5].x >= pixelX &&
            vertexCoordinates_[5].y >= pixelY;

    boost::shared_ptr<ui::Texture> tex = getIngameTexture();
    if (coordinateCheck && tex && tex->isReadComplete()) {
        unsigned int texPixelX = pixelX - vertexCoordinates_[0].x;
        unsigned int texPixelY = pixelY - vertexCoordinates_[0].y;
        return tex->hasPixel(texPixelX, texPixelY);
    } else {
        return false;
    }
}

const CL_Vec3f* IngameObject::getVertexNormals() const {
    return vertexNormals_;
}

void IngameObject::onClick() {
}

void IngameObject::onDoubleClick() {
    setVisible(false);
}

bool IngameObject::isMirrored() const {
    return false;
}

bool IngameObject::isDraggable() const {
    return draggable_;
}

void IngameObject::onDraggedOnto(boost::shared_ptr<IngameObject> obj) {
    LOG_ERROR << "Undraggable object dragged on other object" << std::endl;
}

void IngameObject::onDraggedToVoid() {
    LOG_ERROR << "Undraggable object dragged on void" << std::endl;
}

void IngameObject::onStartDrag(const CL_Point& mousePos) {
    LOG_ERROR << "Starting to drag object other than DynamicItem or Mobile" << std::endl;
}

boost::shared_ptr<IngameObject> IngameObject::getTopParent() {
    boost::shared_ptr<IngameObject> parent = parentObject_.lock();
    if (parent) {
        return parent->getTopParent();
    } else {
        return shared_from_this();
    }
}

void IngameObject::printRenderPriority() {
    LOG_DEBUG << "Render priority: " << renderPriority_[0] << " - "
            << renderPriority_[1] << " - "
            << renderPriority_[2] << " - "
            << renderPriority_[3] << " - "
            << renderPriority_[4] << " - "
            << renderPriority_[5] << " - " << std::endl;
}

void IngameObject::setOverheadMessageOffsets() {
    std::list<boost::shared_ptr<IngameObject> >::reverse_iterator iter = childObjects_.rbegin();
    std::list<boost::shared_ptr<IngameObject> >::reverse_iterator end = childObjects_.rend();

    int offset = -5;

    for (; iter != end; ++iter) {
        if ((*iter)->isSpeech()) {
            boost::shared_ptr<OverheadMessage> msgObj = boost::dynamic_pointer_cast<OverheadMessage>(*iter);
            int curHeight = (*iter)->getIngameTexture()->getHeight();
            int curOffset = offset - curHeight;
            msgObj->setParentPixelOffset(curOffset);
            offset = curOffset - 2;
        }
    }
}

bool IngameObject::isInRenderQueue(boost::shared_ptr<ui::RenderQueue> rq) {
    std::list<boost::weak_ptr<ui::RenderQueue> >::const_iterator iter = renderQueues_.begin();
    std::list<boost::weak_ptr<ui::RenderQueue> >::const_iterator end = renderQueues_.end();
    for (; iter != end; ++iter) {
        if (iter->lock() == rq) {
            return true;
        }
    }

    return false;
}

void IngameObject::addToRenderQueue(boost::shared_ptr<ui::RenderQueue> rq) {
    boost::weak_ptr<ui::RenderQueue> weakRq(rq);

    if (!isInRenderQueue(rq)) {
        renderQueues_.push_back(weakRq);
        rq->add(shared_from_this());
    }
}

void IngameObject::removeFromRenderQueue(boost::shared_ptr<ui::RenderQueue> rq) {
    std::list<boost::weak_ptr<ui::RenderQueue> >::iterator iter = renderQueues_.begin();
    std::list<boost::weak_ptr<ui::RenderQueue> >::iterator end = renderQueues_.end();
    for (; iter != end; ++iter) {
        if (iter->lock() == rq) {
            break;
        }
    }

    if (iter != end) {
        renderQueues_.erase(iter);
        rq->remove(shared_from_this());
    }
}

void IngameObject::addChildObject(boost::shared_ptr<IngameObject> obj) {
    std::list<boost::shared_ptr<IngameObject> >::iterator iter = std::find(childObjects_.begin(), childObjects_.end(), obj);

    if (iter == childObjects_.end()) {
        obj->setParentObject(shared_from_this());
        childObjects_.push_back(obj);

        if (obj->isSpeech()) {
            setOverheadMessageOffsets();
        }
    }
}

void IngameObject::removeChildObject(boost::shared_ptr<IngameObject> obj) {
    std::list<boost::shared_ptr<IngameObject> >::iterator iter = std::find(childObjects_.begin(), childObjects_.end(), obj);

    if (iter != childObjects_.end()) {
        obj->setParentObject();
        childObjects_.erase(iter);
    }
}

void IngameObject::onAddedToParent() {
}

void IngameObject::onRemovedFromParent() {
}

void IngameObject::setParentObject() {
    boost::shared_ptr<IngameObject> parent = parentObject_.lock();

    if (parent && (isSpeech() || parent->isMobile())) {
        // remove this item from all render queues the parent is in
        std::list<boost::weak_ptr<ui::RenderQueue> >::iterator iter = renderQueues_.begin();
        std::list<boost::weak_ptr<ui::RenderQueue> >::iterator end = renderQueues_.end();
        std::list<boost::weak_ptr<ui::RenderQueue> >::iterator helper;
        while (iter != end) {
            boost::shared_ptr<ui::RenderQueue> rq = iter->lock();
            if (parent->isInRenderQueue(rq)) {
                helper = iter;
                ++iter;
                renderQueues_.erase(helper);
            } else {
                ++iter;
            }
        }
    }

    onRemovedFromParent();

    parentObject_.reset();
}

void IngameObject::setParentObject(boost::shared_ptr<IngameObject> parent) {
    boost::shared_ptr<IngameObject> curParent = parentObject_.lock();
    if (curParent) {
        setParentObject();
    }

    if (isSpeech() || parent->isMobile()) {
        // add this item to all render queues the parent is in
        std::list<boost::weak_ptr<ui::RenderQueue> >::iterator iter = parent->renderQueues_.begin();
        std::list<boost::weak_ptr<ui::RenderQueue> >::iterator end = parent->renderQueues_.end();
        for (; iter != end; ++iter) {
            addToRenderQueue(iter->lock());
        }
    }

    parentObject_ = parent;

    onAddedToParent();
}

bool IngameObject::isMap() const {
    return objectType_ == TYPE_MAP;
}

bool IngameObject::isStaticItem() const {
    return objectType_ == TYPE_STATIC_ITEM;
}

bool IngameObject::isDynamicItem() const {
    return objectType_ == TYPE_DYNAMIC_ITEM;
}

bool IngameObject::isMobile() const {
    return objectType_ == TYPE_MOBILE;
}

bool IngameObject::isSpeech() const {
    return objectType_ == TYPE_SPEECH;
}

void IngameObject::onDelete() {
    boost::shared_ptr<IngameObject> parent = parentObject_.lock();
    if (parent) {
        parent->removeChildObject(shared_from_this());
    }
}

}
}
