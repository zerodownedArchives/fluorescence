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
