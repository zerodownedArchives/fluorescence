
#include "mobile.hpp"

#include <misc/log.hpp>

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
    LOG_DEBUG << "Clicked on mobile!" << std::endl;
}

void Mobile::onDoubleClick() {
    LOG_DEBUG << "Doubleclicked on mobile!" << std::endl;
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
}

void Mobile::updateVertexCoordinates() {
    ui::AnimationFrame frame = textureProvider_->getCurrentFrame();
    int texWidth = frame.texture_->getWidth();
    int texHeight = frame.texture_->getHeight();


    //int px = (getLocX() - getLocY()) * 22 - texWidth/2 + 22;
    //int py = (getLocX() + getLocY()) * 22 - texHeight + 44;
    //py -= getLocZ() * 4;

    int px = (getLocX() - getLocY()) * 22;
    int py = (getLocX() + getLocY()) * 22;
    py -= getLocZ() * 4;

    px -= frame.centerX_;
    py = py - texHeight - frame.centerY_;
    //py -= frame.centerY_;

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

}
}
