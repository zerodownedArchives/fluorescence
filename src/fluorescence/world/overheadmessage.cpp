
#include "overheadmessage.hpp"

#include <ui/texture.hpp>
#include <ui/manager.hpp>
#include <ui/fontengine.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace world {

OverheadMessage::OverheadMessage(const UnicodeString& text, unsigned int font, unsigned int color, bool useRgbColor) :
            IngameObject(IngameObject::TYPE_SPEECH) {
    texture_ = ui::Manager::getFontEngine()->getUniFontTexture(font, text, 180, color, useRgbColor);
}

boost::shared_ptr<ui::Texture> OverheadMessage::getIngameTexture() const {
    return texture_;
}

void OverheadMessage::updateVertexCoordinates() {
    // do nothing
    CL_Vec2f parentCoords = parentObject_.lock()->getVertexCoordinates()[0];

    int x = parentCoords.x + 22 - texture_->getWidth()/2;
    int y = parentCoords.y + parentPixelOffsetY_;

    CL_Rectf rect(x, y, CL_Sizef(texture_->getWidth(), texture_->getHeight()));

    worldRenderData_.vertexCoordinates_[0] = CL_Vec2f(rect.left, rect.top);
    worldRenderData_.vertexCoordinates_[1] = CL_Vec2f(rect.right, rect.top);
    worldRenderData_.vertexCoordinates_[2] = CL_Vec2f(rect.left, rect.bottom);
    worldRenderData_.vertexCoordinates_[3] = CL_Vec2f(rect.right, rect.top);
    worldRenderData_.vertexCoordinates_[4] = CL_Vec2f(rect.left, rect.bottom);
    worldRenderData_.vertexCoordinates_[5] = CL_Vec2f(rect.right, rect.bottom);
}

void OverheadMessage::updateRenderPriority() {
    // Move to front
    // TODO: Handle mouse over
    worldRenderData_.renderPriority_[0] = 0xFFFFFFF;
}

void OverheadMessage::updateTextureProvider() {
    // do nothing
}

bool OverheadMessage::updateAnimation(unsigned int elapsedMillis) {
    // do nothing
    return false;
}

void OverheadMessage::onClick() {
    LOG_DEBUG << "Overhead message clicked" << std::endl;
}

void OverheadMessage::setParentPixelOffset(int y) {
    parentPixelOffsetY_ = y;

    invalidateVertexCoordinates();
}

void OverheadMessage::onAddedToParent() {
    boost::shared_ptr<IngameObject> parent = parentObject_.lock();

    if (parent) {
        std::list<boost::shared_ptr<ui::RenderQueue> >::iterator iter = parent->rqBegin();
        std::list<boost::shared_ptr<ui::RenderQueue> >::iterator end = parent->rqEnd();
        for (; iter != end; ++iter) {
            addToRenderQueue(*iter);
        }
    }
}

void OverheadMessage::onRemovedFromParent() {
    boost::shared_ptr<IngameObject> parent = parentObject_.lock();

    // remove this item from all render queues the parent is in
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator iter = rqBegin();
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator end = rqEnd();

    std::list<boost::shared_ptr<ui::RenderQueue> > rqsToRemove;
    for (; iter != end; ++iter) {
        if (parent->isInRenderQueue(*iter)) {
            rqsToRemove.push_back(*iter);
        }
    }

    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator remIter = rqsToRemove.begin();
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator remEnd = rqsToRemove.end();
    for (; remIter != remEnd; ++remIter) {
        removeFromRenderQueue(*remIter);
    }
}

}
}
