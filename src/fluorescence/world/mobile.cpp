/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "mobile.hpp"

#include <misc/log.hpp>

#include <net/manager.hpp>
#include <net/packets/bf.hpp>
#include <net/packets/bf/13_contextmenurequest.hpp>
#include <net/packets/06_doubleclick.hpp>
#include <net/packets/34_statskillquery.hpp>
#include <net/packets/72_warmode.hpp>
#include <net/packets/05_attackrequest.hpp>

#include <data/manager.hpp>
#include <data/huesloader.hpp>

#include <ui/manager.hpp>
#include <ui/gumpmenu.hpp>
#include <ui/cursormanager.hpp>
#include <ui/components/paperdollview.hpp>
#include <ui/singletextureprovider.hpp>
#include <ui/animtextureprovider.hpp>

#include "manager.hpp"
#include "dynamicitem.hpp"
#include "sector.hpp"
#include "smoothmovement.hpp"
#include "smoothmovementmanager.hpp"

namespace fluo {
namespace world {

Mobile::Mobile(Serial serial) : ServerObject(serial, IngameObject::TYPE_MOBILE), baseBodyId_(0), bodyId_(0), isWarMode_(false) {
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
    LOG_INFO << "Clicked mobile, id=" << std::hex << getBodyId() << std::dec << " loc=(" << getLocXGame() << "/" << getLocYGame() << "/" <<
            getLocZGame() << ")" << std::endl;

    printRenderDepth();

    std::list<boost::shared_ptr<IngameObject> >::iterator iter = childObjects_.begin();
    std::list<boost::shared_ptr<IngameObject> >::iterator end = childObjects_.end();

    for (; iter != end; ++iter) {
        boost::shared_ptr<DynamicItem> dyn = boost::dynamic_pointer_cast<DynamicItem>(*iter);
        if (dyn && dyn->getLayer() == Layer::MOUNT) {
            dyn->onClick();
            //LOG_DEBUG << "dyn has sector: " << dyn->hasSector() << std::endl;
        }
    }

    //net::packets::SingleClick pkt(getSerial());

    boost::shared_ptr<net::Packet> subPacket(new net::packets::bf::ContextMenuRequest(getSerial()));
    net::packets::BF pkt(subPacket);
    net::Manager::getSingleton()->send(pkt);
}

void Mobile::onDoubleClick() {
    LOG_INFO << "Double clicked mobile, id=" << std::hex << getBodyId() << std::dec << " loc=(" << getLocXGame() << "/" << getLocYGame() << "/" <<
            getLocZGame() << ")" << std::endl;

    world::Mobile* player = world::Manager::getSingleton()->getPlayer().get();
    if (player->isWarMode() && player != this) {
        net::packets::AttackRequest pkt(getSerial());
        net::Manager::getSingleton()->send(pkt);
    } else {
        net::packets::DoubleClick pkt(getSerial());
        net::Manager::getSingleton()->send(pkt);
    }
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

        animType_ = data::Manager::getAnimType(bodyId_);

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

    int px = (getLocXDraw() - getLocYDraw()) * 22 + 22;
    int py = (getLocXDraw() + getLocYDraw()) * 22 - getLocZDraw() * 4 + 22;
    py = py - frame.centerY_ - texHeight;

    if (isMirrored()) {
        px = px - texWidth + frame.centerX_;
    } else {
        px -= frame.centerX_;
    }

    CL_Rectf rect(px, py, px + texWidth, py + texHeight);

    worldRenderData_.setVertexCoordinates(rect);
}

void Mobile::updateRenderDepth() {
    int8_t z = getLocZGame() + 7;

    worldRenderData_.setRenderDepth(getLocXGame(), getLocYGame(), z, 30, 0, getSerial() & 0xFF);
}

void Mobile::updateTextureProvider() {
    textureProvider_.reset(new ui::AnimTextureProvider(bodyId_, getIdleAnim()));
    textureProvider_->setDirection(direction_);

    data::PaperdollDef pdDef = data::Manager::getPaperdollDef(bodyId_);
    if (pdDef.bodyId_ == 0) {
        LOG_ERROR << "Unable to find paperdoll.def entry for body " << bodyId_ << std::endl;
        gumpTextureProvider_.reset(new ui::SingleTextureProvider(data::TextureSource::GUMPART, 0xC));
    } else {
        gumpTextureProvider_.reset(new ui::SingleTextureProvider(data::TextureSource::GUMPART, pdDef.gumpId_));

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

void Mobile::animate(unsigned int animId, unsigned int delay, unsigned int repeatMode) {
    if (!textureProvider_) {
        // not yet initialized
        return;
    }

    textureProvider_->setAnimId(animId);
    textureProvider_->setRepeatMode(repeatMode);
    textureProvider_->setDelay(delay);

    std::list<boost::shared_ptr<IngameObject> >::iterator iter = childObjects_.begin();
    std::list<boost::shared_ptr<IngameObject> >::iterator end = childObjects_.end();

    for (; iter != end; ++iter) {
        if ((*iter)->isDynamicItem()) {
            boost::static_pointer_cast<DynamicItem>(*iter)->animate(animId, delay, repeatMode);
        }
    }
}

void Mobile::stopAnim() {
    animate(getIdleAnim(), 0, AnimRepeatMode::LOOP);
}

void Mobile::setDirection(unsigned int direction) {
    isRunning_ = (direction & Direction::RUNNING) != 0;
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
            boost::static_pointer_cast<DynamicItem>(*iter)->setDirection(direction_);
        }
    }
}

unsigned int Mobile::getDirection() const {
    return direction_;
}

bool Mobile::isRunning() const {
    return isRunning_;
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
    if (!menu) {
        return;
    }

    linkedGumps_.push_back(menu);
    menu->setLinkedMobile(this);
}

void Mobile::removeLinkedGump(ui::GumpMenu* menu) {
    menu->setLinkedMobile(NULL);
    linkedGumps_.remove(menu);
}

void Mobile::onStartDrag(const CL_Point& mousePos) {
    ui::Manager::getSingleton()->getCursorManager()->stopDragging();

    openStatusGump(mousePos);
}

void Mobile::openStatusGump(const CL_Point& mousePos) {
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

void Mobile::openProfile() {
    if (isPlayer()) {
        findOrCreateLinkedGump("profile-self");
    } else {
        findOrCreateLinkedGump("profile-other");
    }
}

void Mobile::openSkillsGump() {
    ui::GumpMenu* menu = findLinkedGump("skills");

    if (!menu) {
        menu = ui::GumpMenus::openSkills(this);
        addLinkedGump(menu);
    }

    onPropertyUpdate();

    // not necessary, skills are always up to date
    //net::packets::StatSkillQuery queryPacket(getSerial(), net::packets::StatSkillQuery::QUERY_SKILLS);
    //net::Manager::getSingleton()->send(queryPacket);
}

ui::GumpMenu* Mobile::findLinkedGump(const UnicodeString& gumpName) {
    std::list<ui::GumpMenu*>::iterator iter = linkedGumps_.begin();
    std::list<ui::GumpMenu*>::iterator end = linkedGumps_.end();

    for (; iter != end; ++iter) {
        if ((*iter)->getName() == gumpName) {
            (*iter)->bring_to_front();
            return *iter;
        }
    }

    return nullptr;
}

ui::GumpMenu* Mobile::findOrCreateLinkedGump(const UnicodeString& gumpName) {
    ui::GumpMenu* menu = findLinkedGump(gumpName);

    if (!menu) {
        // not found, create
        menu = ui::Manager::getSingleton()->openXmlGump(gumpName);
        addLinkedGump(menu);
    }

    return menu;
}

void Mobile::openPaperdoll() {
    ui::GumpMenu* paperdoll;
    if (isPlayer()) {
        paperdoll = findOrCreateLinkedGump("paperdoll-self");
    } else {
        paperdoll = findOrCreateLinkedGump("paperdoll-other");
    }

    ui::components::PaperdollView* pdView = dynamic_cast<ui::components::PaperdollView*>(paperdoll->get_named_item("paperdoll"));
    if (pdView) {
        boost::shared_ptr<Mobile> sharedThis = boost::static_pointer_cast<Mobile>(shared_from_this());
        pdView->addObject(sharedThis);
        pdView->setMobile(sharedThis);
    } else {
        LOG_ERROR << "Unable to find paperdoll component in paperdoll gump" << std::endl;
    }
}

void Mobile::setRace(unsigned int race) {
    if (race_ != race) {
        race_ = race;
    }

    // TODO: what does this affect?
}

unsigned int Mobile::getRace() const {
    return race_;
}

void Mobile::setGender(unsigned int gender) {
    // TODO: what does this affect?
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

    ServerObject::onDelete();
}

void Mobile::onChildObjectAdded(boost::shared_ptr<IngameObject> obj) {
    // add this item to all render queues the mobile is in
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator iter = rqBegin();
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator end = rqEnd();
    for (; iter != end; ++iter) {
        obj->addToRenderQueue(*iter);
    }

    if (sector_) {
        obj->setSector(sector_);
    }

    if (obj->isDynamicItem()) {
        boost::static_pointer_cast<DynamicItem>(obj)->setDirection(getDirection());
    }

    updateIdleAnim();
    repaintRectangle(true);
}

void Mobile::onBeforeChildObjectRemoved(boost::shared_ptr<IngameObject> obj) {
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator iter = rqBegin();
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator end = rqEnd();

    std::list<boost::shared_ptr<ui::RenderQueue> > rqsToRemove;
    for (; iter != end; ++iter) {
        if (obj->isInRenderQueue(*iter)) {
            obj->removeFromRenderQueue(*iter);
        }
    }

    if (sector_) {
        sector_->removeDynamicObject(obj.get());
    }
}

void Mobile::onAfterChildObjectRemoved() {
    updateIdleAnim();

    repaintRectangle(true);
}

void Mobile::updateIdleAnim() {
    unsigned int idleAnim = getIdleAnim();
    if (textureProvider_) {
        textureProvider_->setDefaultAnimId(idleAnim);
    }

    std::list<boost::shared_ptr<IngameObject> >::iterator iter = childObjects_.begin();
    std::list<boost::shared_ptr<IngameObject> >::iterator end = childObjects_.end();

    for (; iter != end; ++iter) {
        if ((*iter)->isDynamicItem()) {
             boost::static_pointer_cast<DynamicItem>(*iter)->setIdleAnim(idleAnim);
        }
    }
}

bool Mobile::isMounted() const {
    return hasItemOnLayer(Layer::MOUNT);
}

bool Mobile::isWarMode() const {
    return isWarMode_;
}

unsigned int Mobile::getMoveAnim() const {
    switch (animType_) {
        case AnimType::HIGH_DETAIL:
            return 0;
        case AnimType::LOW_DETAIL:
            return isRunning_ ? 1 : 0;
        default:
        case AnimType::PEOPLE:
            if (isMounted()) {
                return isRunning_ ? 24 : 23;
            } else if (isWarMode_) {
                if (!isRunning_) {
                    return 15;
                } else if (hasItemOnLayer(Layer::ONEHANDED) || hasItemOnLayer(Layer::TWOHANDED)) {
                    return 3;
                } else {
                    return 2;
                }
            } else if (hasItemOnLayer(Layer::ONEHANDED) || hasItemOnLayer(Layer::TWOHANDED)) {
                return isRunning_ ? 3 : 1;
            } else {
                return isRunning_ ? 2 : 0;
            }
    }
}

unsigned int Mobile::getIdleAnim() const {
    switch (animType_) {
        case AnimType::HIGH_DETAIL:
            return 1;
        case AnimType::LOW_DETAIL:
            return 2;
        default:
        case AnimType::PEOPLE:
            if (isMounted()) {
                return 25;
            } else if (isWarMode_) {
                if (hasItemOnLayer(Layer::TWOHANDED)) {
                    return 8;
                } else {
                    return 7;
                }
            } else {
                return 4;
            }
    }
}

void Mobile::onAddedToSector(world::Sector* sector) {
    std::list<boost::shared_ptr<IngameObject> >::iterator iter = childObjects_.begin();
    std::list<boost::shared_ptr<IngameObject> >::iterator end = childObjects_.end();

    for (; iter != end; ++iter) {
        sector->addDynamicObject(iter->get());
    }
}

void Mobile::onRemovedFromSector(world::Sector* sector) {
    std::list<boost::shared_ptr<IngameObject> >::iterator iter = childObjects_.begin();
    std::list<boost::shared_ptr<IngameObject> >::iterator end = childObjects_.end();

    for (; iter != end; ++iter) {
        sector->removeDynamicObject(iter->get());
    }
}

bool Mobile::hasItemOnLayer(unsigned int layer) const {
    std::list<boost::shared_ptr<IngameObject> >::const_iterator iter = childObjects_.begin();
    std::list<boost::shared_ptr<IngameObject> >::const_iterator end = childObjects_.end();

    for (; iter != end; ++iter) {
        if ((*iter)->isDynamicItem() && boost::static_pointer_cast<DynamicItem>(*iter)->getLayer() == layer) {
            return true;
        }
    }

    return false;
}

void Mobile::setWarMode(bool warMode) {
    if (warMode != isWarMode_) {
        isWarMode_ = warMode;
        invalidateTextureProvider();

        if (isPlayer()) {
            net::packets::WarMode pkt(isWarMode_);
            net::Manager::getSingleton()->send(pkt);

            ui::Manager::getCursorManager()->setWarMode(isWarMode_);
        }

        onPropertyUpdate();
    }
}

void Mobile::setStatusFlags(uint8_t flags) {
    setWarMode(flags & MobileStatusFlags::WARMODE);

    // TODO: handle other flags
}

void Mobile::displayStatChange(unsigned int str, unsigned int dex, unsigned int intel) {
    if (!hasProperty("strength")) {
        // avoid notice on init
        return;
    }

    unsigned int oldStr = getProperty("strength").asInt();
    unsigned int oldDex = getProperty("dexterity").asInt();
    unsigned int oldIntel = getProperty("intelligence").asInt();

    std::stringstream sstr;

    if (str != oldStr) {
        int diff = str - oldStr;
        sstr << "Your strength has changed by " << diff << ". It is now " << str << ".";
        UnicodeString msg(sstr.str().c_str());
        world::Manager::getSingleton()->systemMessage(msg, 64);
    }

    if (dex != oldDex) {
        int diff = dex - oldDex;
        sstr.str();
        sstr << "Your dexterity has changed by " << diff << ". It is now " << dex << ".";
        UnicodeString msg(sstr.str().c_str());
        world::Manager::getSingleton()->systemMessage(msg, 64);
    }

    if (intel != oldIntel) {
        int diff = intel - oldIntel;
        sstr.str();
        sstr << "Your intelligence has changed by " << diff << ". It is now " << intel << ".";
        UnicodeString msg(sstr.str().c_str());
        world::Manager::getSingleton()->systemMessage(msg, 64);
    }
}

unsigned int Mobile::getMovementDuration() const {
    if (isMounted()) {
        if (isRunning_) {
            return 95;
        } else {
            return 185;
        }
    } else {
        if (isRunning_) {
            return 175;
        } else {
            return 375;
        }
    }
}

void Mobile::onLocationChanged(const CL_Vec3f& oldLocation) {
    if (isPlayer()) {
        world::Manager::getSingleton()->updateRoofHeight();
    }

    // pass call to parent class
    ServerObject::onLocationChanged(oldLocation);
}

void Mobile::moveTo(unsigned int locX, unsigned int locY, int locZ, unsigned int direction) {
    CL_Vec3f smoothTargetLoc = world::Manager::getSmoothMovementManager()->getTargetLoc(getSerial());
    int diffx, diffy;
    if (smoothTargetLoc.x != 0 && smoothTargetLoc.y != 0) {
        diffx = abs(ceil(smoothTargetLoc.x) - locX);
        diffy = abs(ceil(smoothTargetLoc.y) - locY);
    } else {
        diffx = abs((int)getLocXGame() - (int)locX);
        diffy = abs((int)getLocYGame() - (int)locY);
    }

    if (diffx > 1 || diffy > 1) {
        // far teleport
        setLocation(locX, locY, locZ);
        setDirection(direction);
    } else if (diffx != 0 || diffy != 0) {
        boost::shared_ptr<ServerObject> sharedThis = boost::static_pointer_cast<ServerObject>(shared_from_this());
        world::SmoothMovement mov(sharedThis, CL_Vec3f(locX, locY, locZ), getMovementDuration());
        mov.setStartCallback(boost::bind(&Mobile::resumeAnimationCallback, this));
        mov.setFinishedCallback(boost::bind(&Mobile::haltAnimationCallback, this));
        world::Manager::getSmoothMovementManager()->add(getSerial(), mov);
        setDirection(direction);
    } else {
        // only direction change
        boost::shared_ptr<ServerObject> sharedThis = boost::static_pointer_cast<ServerObject>(shared_from_this());
        world::SmoothMovement dirCh(sharedThis, direction);
        world::Manager::getSmoothMovementManager()->add(getSerial(), dirCh);
    }
}

void Mobile::haltAnimationCallback() {
    if (textureProvider_) {
        textureProvider_->halt();
    }
}

void Mobile::resumeAnimationCallback() {
    if (textureProvider_ && textureProvider_->isHalted()) {
        textureProvider_->resume();
    } else {
        animate(getMoveAnim(), 0, AnimRepeatMode::LOOP);
    }
}

}
}
