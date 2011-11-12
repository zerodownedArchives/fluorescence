
#include "mobile.hpp"

#include "dynamicitem.hpp"

#include <misc/log.hpp>

#include <net/manager.hpp>
#include <net/packets/bf.hpp>
#include <net/packets/bf/contextmenurequest.hpp>
#include <net/packets/doubleclick.hpp>
#include <net/packets/statskillquery.hpp>

#include <data/manager.hpp>
#include <data/huesloader.hpp>

#include <ui/manager.hpp>
#include <ui/gumpmenu.hpp>
#include <ui/gumpfactory.hpp>
#include <ui/cursormanager.hpp>
#include <ui/components/gumpview.hpp>
#include <ui/singletextureprovider.hpp>
#include <ui/animtextureprovider.hpp>

#include <world/manager.hpp>


#include "smoothmovement.hpp"
#include "smoothmovementmanager.hpp"

namespace fluo {
namespace world {

Mobile::Mobile(Serial serial) : ServerObject(serial, IngameObject::TYPE_MOBILE), bodyId_(0), isMounted_(false), isArmed_(false), isWarmode_(false) {
}

boost::shared_ptr<ui::Texture> Mobile::getIngameTexture() const {
    return textureProvider_->getTexture();
}

boost::shared_ptr<ui::Texture> Mobile::getGumpTexture() const {
    return gumpTextureProvider_->getTexture();
}

unsigned int Mobile::getBodyId() const {
    return bodyId_;
}

unsigned int Mobile::getBaseBodyId() const {
    return baseBodyId_;
}

void Mobile::onClick() {
    LOG_INFO << "Clicked mobile, id=" << std::hex << getBodyId() << std::dec << " loc=(" << getLocX() << "/" << getLocY() << "/" <<
            getLocZ() << ")" << std::endl;

    printRenderPriority();

    //net::packets::SingleClick pkt(getSerial());

    boost::shared_ptr<net::Packet> subPacket(new net::packets::bf::ContextMenuRequest(getSerial()));
    net::packets::BF pkt(subPacket);
    net::Manager::getSingleton()->send(pkt);
}

void Mobile::onDoubleClick() {
    LOG_INFO << "Double clicked mobile, id=" << std::hex << getBodyId() << std::dec << " loc=(" << getLocX() << "/" << getLocY() << "/" <<
            getLocZ() << ")" << std::endl;

    net::packets::DoubleClick pkt(getSerial());
    net::Manager::getSingleton()->send(pkt);
}

void Mobile::setBodyId(unsigned int value) {
    if (value != baseBodyId_) {
        baseBodyId_ = value;

        data::BodyDef bodyDefData = data::Manager::getBodyDef(baseBodyId_);
        if (bodyDefData.origBody_ == baseBodyId_) {
            bodyId_ = bodyDefData.newBody_;
        } else {
            bodyId_ = baseBodyId_;
        }

        invalidateTextureProvider();
    }
}

void Mobile::updateVertexCoordinates() {
    ui::AnimationFrame frame = textureProvider_->getCurrentFrame();
    int texWidth = frame.texture_->getWidth();
    int texHeight = frame.texture_->getHeight();


    //int px = (getLocX() - getLocY()) * 22 - texWidth/2 + 22;
    //int py = (getLocX() + getLocY()) * 22 - texHeight + 44;
    //py -= getLocZ() * 4;

    int px = (getLocX() - getLocY()) * 22 + 22;
    int py = (getLocX() + getLocY()) * 22 - getLocZ() * 4 + 22;
    py = py - frame.centerY_ - texHeight;

    if (isMirrored()) {
        px = px - texWidth + frame.centerX_;
    } else {
        px -= frame.centerX_;
    }

    CL_Rectf rect(px, py, px + texWidth, py + texHeight);

    worldRenderData_.vertexCoordinates_[0] = CL_Vec2f(rect.left, rect.top);
    worldRenderData_.vertexCoordinates_[1] = CL_Vec2f(rect.right, rect.top);
    worldRenderData_.vertexCoordinates_[2] = CL_Vec2f(rect.left, rect.bottom);
    worldRenderData_.vertexCoordinates_[3] = CL_Vec2f(rect.right, rect.top);
    worldRenderData_.vertexCoordinates_[4] = CL_Vec2f(rect.left, rect.bottom);
    worldRenderData_.vertexCoordinates_[5] = CL_Vec2f(rect.right, rect.bottom);
}

void Mobile::updateRenderPriority() {
    // render prio
    // level 0 x+y
    worldRenderData_.renderPriority_[0] = ceilf(getLocX()) + ceilf(getLocY());

    // level 1 z
    worldRenderData_.renderPriority_[1] = ceilf(getLocZ()) + 7;

    // level 2 type of object (map behind statics behind dynamics behind mobiles if on same coordinates)
    worldRenderData_.renderPriority_[2] = 30;

    // level 2 layer
    worldRenderData_.renderPriority_[3] = 0;

    // level 5 serial
    worldRenderData_.renderPriority_[5] = getSerial();
}

void Mobile::updateTextureProvider() {
    textureProvider_.reset(new ui::AnimTextureProvider(bodyId_));
    textureProvider_->setDirection(direction_);

    data::PaperdollDef pdDef = data::Manager::getPaperdollDef(bodyId_);
    if (pdDef.bodyId_ == 0) {
        LOG_ERROR << "Unable to find paperdoll.def entry for body " << bodyId_ << std::endl;
        gumpTextureProvider_.reset(new ui::SingleTextureProvider(ui::SingleTextureProvider::FROM_GUMPART_MUL, 0xC));
    } else {
        gumpTextureProvider_.reset(new ui::SingleTextureProvider(ui::SingleTextureProvider::FROM_GUMPART_MUL, pdDef.gumpId_));

        if (!childObjects_.empty()) {
            std::list<boost::shared_ptr<IngameObject> >::iterator iter = childObjects_.begin();
            std::list<boost::shared_ptr<IngameObject> >::iterator end = childObjects_.end();

            for (; iter != end; ++iter) {
                (*iter)->invalidateTextureProvider();
            }
        }
    }
}

bool Mobile::updateAnimation(unsigned int elapsedMillis) {
    return textureProvider_->update(elapsedMillis);
}

void Mobile::playAnim(unsigned int animId) {
    textureProvider_->setAnimId(animId);

    std::list<boost::shared_ptr<IngameObject> >::iterator iter = childObjects_.begin();
    std::list<boost::shared_ptr<IngameObject> >::iterator end = childObjects_.end();

    for (; iter != end; ++iter) {
        if ((*iter)->isDynamicItem()) {
            boost::dynamic_pointer_cast<DynamicItem>(*iter)->playAnim(animId);
        }
    }
}

void Mobile::setDirection(unsigned int direction) {
    isRunning_ = direction_ & Direction::RUNNING;
    direction_ = direction & 0x7;

    if (textureProvider_) {
        textureProvider_->setDirection(direction);
        invalidateVertexCoordinates();
        forceRepaint();
    }

    std::list<boost::shared_ptr<IngameObject> >::iterator iter = childObjects_.begin();
    std::list<boost::shared_ptr<IngameObject> >::iterator end = childObjects_.end();

    for (; iter != end; ++iter) {
        if ((*iter)->isDynamicItem()) {
            boost::dynamic_pointer_cast<DynamicItem>(*iter)->setDirection(direction_);
        }
    }
}

unsigned int Mobile::getDirection() const {
    return direction_;
}

bool Mobile::isMirrored() const {
    return direction_ < 3;
}

Variable& Mobile::getProperty(const UnicodeString& name) {
    return propertyMap_[name];
}

bool Mobile::hasProperty(const UnicodeString& name) const {
    return propertyMap_.find(name) != propertyMap_.end();
}

void Mobile::onPropertyUpdate() {
    // iterate over all gumps related to this mobile and call updateproperty on all property related components
    std::list<ui::GumpMenu*>::iterator iter = linkedGumps_.begin();
    std::list<ui::GumpMenu*>::iterator end = linkedGumps_.end();

    for (; iter != end; ++iter) {
        (*iter)->updateMobileProperties();
    }
}

void Mobile::addLinkedGump(ui::GumpMenu* menu) {
    linkedGumps_.push_back(menu);
    menu->setLinkedMobile(this);
}

void Mobile::removeLinkedGump(ui::GumpMenu* menu) {
    menu->setLinkedMobile(NULL);
    linkedGumps_.remove(menu);
}

void Mobile::onStartDrag(const CL_Point& mousePos) {
    ui::Manager::getSingleton()->getCursorManager()->stopDragging();

    ui::GumpMenu* statsMenu;
    if (isPlayer()) {
        statsMenu = findOrCreateLinkedGump("status-self");
    } else {
        statsMenu = findOrCreateLinkedGump("status-other");
    }

    CL_Size size = statsMenu->get_size();
    statsMenu->set_window_geometry(CL_Rect(mousePos.x - 10, mousePos.y - 10, size));

    if (statsMenu->isDraggable()) {
        statsMenu->startDragging(statsMenu->screen_to_component_coords(mousePos));
    }

    net::packets::StatSkillQuery queryPacket(getSerial(), net::packets::StatSkillQuery::QUERY_STATS);
    net::Manager::getSingleton()->send(queryPacket);
}

ui::GumpMenu* Mobile::findOrCreateLinkedGump(const UnicodeString& gumpName) {
    std::list<ui::GumpMenu*>::iterator iter = linkedGumps_.begin();
    std::list<ui::GumpMenu*>::iterator end = linkedGumps_.end();

    for (; iter != end; ++iter) {
        if ((*iter)->getName() == gumpName) {
            (*iter)->bring_to_front();
            return *iter;
        }
    }

    // not found, create
    ui::GumpMenu* menu = ui::Manager::getSingleton()->openXmlGump(gumpName);
    addLinkedGump(menu);

    return menu;
}

void Mobile::openPaperdoll() {
    ui::GumpMenu* paperdoll;
    if (isPlayer()) {
        paperdoll = findOrCreateLinkedGump("paperdoll-self");
    } else {
        paperdoll = findOrCreateLinkedGump("paperdoll-other");
    }

    ui::components::GumpView* pdView = dynamic_cast<ui::components::GumpView*>(paperdoll->get_named_item("paperdoll"));
    if (pdView) {
        pdView->addObject(shared_from_this());
    } else {
        LOG_ERROR << "Unable to find paperdoll component in paperdoll gump" << std::endl;
    }
}

void Mobile::setRace(unsigned int race) {
    if (race_ != race) {
        race_ = race;
    }

    // TODO
}

unsigned int Mobile::getRace() const {
    return race_;
}

void Mobile::setGender(unsigned int gender) {
    // TODO
}

unsigned int Mobile::isFemale() const {
    return female_;
}

void Mobile::updateGumpTextureProvider() {
}

bool Mobile::isPlayer() const {
    return world::Manager::getSingleton()->getPlayer().get() == this;
}

void Mobile::onDelete() {
    std::list<ui::GumpMenu*>::iterator iter = linkedGumps_.begin();
    std::list<ui::GumpMenu*>::iterator end = linkedGumps_.end();

    for (; iter != end; ++iter) {
        ui::Manager::getSingleton()->closeGumpMenu(*iter);
    }

    linkedGumps_.clear();

    IngameObject::onDelete();
}

void Mobile::onChildObjectAdded(boost::shared_ptr<IngameObject> obj) {
    // add this item to all render queues the mobile is in
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator iter = rqBegin();
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator end = rqEnd();
    for (; iter != end; ++iter) {
        obj->addToRenderQueue(*iter);
    }

    checkItemLayerFlags();
}

void Mobile::onChildObjectRemoved(boost::shared_ptr<IngameObject> obj) {
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator iter = rqBegin();
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator end = rqEnd();

    std::list<boost::shared_ptr<ui::RenderQueue> > rqsToRemove;
    for (; iter != end; ++iter) {
        if (obj->isInRenderQueue(*iter)) {
            rqsToRemove.push_back(*iter);
        }
    }

    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator remIter = rqsToRemove.begin();
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator remEnd = rqsToRemove.end();
    for (; remIter != remEnd; ++remIter) {
        obj->removeFromRenderQueue(*remIter);
    }

    checkItemLayerFlags();
}

bool Mobile::isMounted() const {
    return isMounted_;
}

bool Mobile::isArmed() const {
    return isArmed_;
}

bool Mobile::isWarmode() const {
    return isWarmode_;
}

void Mobile::checkItemLayerFlags() {
    std::list<boost::shared_ptr<IngameObject> >::const_iterator iter = childObjects_.begin();
    std::list<boost::shared_ptr<IngameObject> >::const_iterator end = childObjects_.end();

    bool mnt = false;
    bool arm = false;
    for (; iter != end; ++iter) {
        if ((*iter)->isDynamicItem()) {
            unsigned int layer = boost::dynamic_pointer_cast<DynamicItem>(*iter)->getLayer();
            switch (layer) {
            case Layer::MOUNT:
                mnt = true;
                break;
            case Layer::ONEHANDED:
            case Layer::TWOHANDED:
                arm = true;
                break;
            }
        }
    }

    isMounted_ = mnt;
    isArmed_ = arm;
}

unsigned int Mobile::getWalkAnim() const {
    // TODO: at the moment this is only valid for people
    if (isMounted_) {
        return 23;
    } else if (isWarmode_) {
        return 15;
    } else if (isArmed_) {
        return 1;
    } else {
        return 0;
    }
}

unsigned int Mobile::getRunAnim() const {
    // TODO: at the moment this is only valid for people
    if (isMounted_) {
        return 24;
    } else if (isWarmode_) {
        return 15;
    } else if (isArmed_) {
        return 3;
    } else {
        return 2;
    }
}

}
}
