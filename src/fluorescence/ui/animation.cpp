
#include "animation.hpp"

namespace fluo {
namespace ui {

void Animation::addFrame(const AnimationFrame& frame) {
    frames_.push_back(frame);
}

unsigned int Animation::getFrameCount() const {
    return frames_.size();
}

AnimationFrame Animation::getFrame(unsigned int idx) const {
    AnimationFrame ret;
    if (idx < frames_.size()) {
        ret = frames_[idx];
    }

    return ret;
}

unsigned int Animation::getWidth(unsigned int idx) const {
    unsigned int ret = 0;
    if (idx < frames_.size()) {
        ret = frames_[idx].texture_->getWidth();
    }
    return ret;
}

unsigned int Animation::getHeight(unsigned int idx) const {
    unsigned int ret = 0;
    if (idx < frames_.size()) {
        ret = frames_[idx].texture_->getHeight();
    }
    return ret;
}

bool Animation::hasPixel(unsigned int idx, unsigned int pixelX, unsigned int pixelY) const {
    bool ret = false;
    if (idx < frames_.size()) {
        ret = frames_[idx].texture_->hasPixel(pixelX, pixelY);
    }
    return ret;
}

}
}
