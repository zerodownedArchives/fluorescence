
#include "mobile.hpp"

#include <misc/log.hpp>

#include <net/manager.hpp>
#include <net/packets/singleclick.hpp>
#include <net/packets/doubleclick.hpp>
#include <net/packets/statskillquery.hpp>

#include <data/manager.hpp>
#include <data/huesloader.hpp>

#include <ui/manager.hpp>
#include <ui/gumpmenu.hpp>
#include <ui/gumpfactory.hpp>
#include <ui/cursormanager.hpp>

namespace uome {
namespace world {

Mobile::Mobile(Serial serial) : ServerObject(serial), bodyId_(0) {
}

boost::shared_ptr<ui::Texture> Mobile::getIngameTexture() const {
    return textureProvider_->getTexture();
}

unsigned int Mobile::getBodyId() const {
    return bodyId_;
}

void Mobile::onClick() {
    LOG_INFO << "Clicked mobile, id=" << std::hex << getBodyId() << std::dec << " loc=(" << getLocX() << "/" << getLocY() << "/" <<
            getLocZ() << ")" << std::endl;

    net::packets::SingleClick pkt(getSerial());
    net::Manager::getSingleton()->send(pkt);
}

void Mobile::onDoubleClick() {
    LOG_INFO << "Double clicked mobile, id=" << std::hex << getBodyId() << std::dec << " loc=(" << getLocX() << "/" << getLocY() << "/" <<
            getLocZ() << ")" << std::endl;

    net::packets::DoubleClick pkt(getSerial());
    net::Manager::getSingleton()->send(pkt);
}

void Mobile::setBodyId(unsigned int value) {
    if (value != bodyId_) {
        bodyId_ = value;
        invalidateRenderData(true);
    }

    addToRenderQueue();
}

void Mobile::setHue(unsigned int value) {
    hue_ = value;
    hueInfo_[1] = data::Manager::getHuesLoader()->translateHue(hue_);
}

void Mobile::updateVertexCoordinates() {
    ui::AnimationFrame frame = textureProvider_->getCurrentFrame();
    int texWidth = frame.texture_->getWidth();
    int texHeight = frame.texture_->getHeight();


    //int px = (getLocX() - getLocY()) * 22 - texWidth/2 + 22;
    //int py = (getLocX() + getLocY()) * 22 - texHeight + 44;
    //py -= getLocZ() * 4;

    int px = (getLocX() - getLocY()) * 22 + 22;
    int py = (getLocX() + getLocY()) * 22 - getLocZ() * 4 - 22;
    py = py - frame.centerY_ - texHeight;

    if (isMirrored()) {
        px = px - texWidth + frame.centerX_;
    } else {
        px -= frame.centerX_;
    }

    CL_Rectf rect(px, py, px + texWidth, py + texHeight);

    vertexCoordinates_[0] = CL_Vec2f(rect.left, rect.top);
    vertexCoordinates_[1] = CL_Vec2f(rect.right, rect.top);
    vertexCoordinates_[2] = CL_Vec2f(rect.left, rect.bottom);
    vertexCoordinates_[3] = CL_Vec2f(rect.right, rect.top);
    vertexCoordinates_[4] = CL_Vec2f(rect.left, rect.bottom);
    vertexCoordinates_[5] = CL_Vec2f(rect.right, rect.bottom);
}

void Mobile::updateRenderPriority() {
    // render prio
    // level 0 x+y
    renderPriority_[0] = getLocX() + getLocY();

    // level 1 z
    renderPriority_[1] = getLocZ();

    // level 2 layer
    renderPriority_[2] = 0;

    // level 3 serial
    renderPriority_[3] = getSerial();
}

void Mobile::updateTextureProvider() {
    textureProvider_.reset(new ui::AnimTextureProvider(bodyId_));
    textureProvider_->setDirection(direction_);
}

bool Mobile::updateAnimation(unsigned int elapsedMillis) {
    return textureProvider_->update(elapsedMillis);
}

void Mobile::playAnim(unsigned int animId) {
    textureProvider_->setAnimId(animId);
}

void Mobile::setDirection(unsigned int direction) {
    isRunning_ = direction_ & Direction::RUNNING;
    direction_ = direction & 0x7;

    if (textureProvider_) {
        textureProvider_->setDirection(direction);
    }

    invalidateRenderData();
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

    ui::GumpMenu* statsMenu = ui::Manager::getSingleton()->openXmlGump("smallstatus");
    addLinkedGump(statsMenu);

    CL_Size size = statsMenu->get_size();
    statsMenu->set_window_geometry(CL_Rect(mousePos.x - 10, mousePos.y - 10, size));

    if (statsMenu->isDraggable()) {
        statsMenu->startDragging(statsMenu->screen_to_component_coords(mousePos));
    }

    net::packets::StatSkillQuery queryPacket(getSerial(), net::packets::StatSkillQuery::QUERY_STATS);
    net::Manager::getSingleton()->send(queryPacket);
}

}
}
