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



#include "animtextureprovider.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>

namespace fluo {
namespace ui {

AnimTextureProvider::AnimTextureProvider(unsigned int bodyId, unsigned int defaultAnim) : bodyId_(bodyId), nextAnimId_(0xFFFFFFFFu), currentAnimId_(defaultAnim),
        direction_(0), currentIdx_(0), millis_(0), frameMillis_(100), repeatMode_(AnimRepeatMode::DEFAULT), defaultAnimId_(defaultAnim), nextDirection_(0), haltMillis_(0) {
    animations_[defaultAnimId_] = data::Manager::getAnim(bodyId, defaultAnimId_);
}

boost::shared_ptr<ui::Texture> AnimTextureProvider::getTexture() const {
    //LOG_DEBUG << "getTexture current direction:" << direction_ << std::endl;
    std::map<unsigned int, std::vector<boost::shared_ptr<Animation> > >::const_iterator it = animations_.find(currentAnimId_);
    if (it != animations_.end() && it->second[direction_] && it->second[direction_]->isReadComplete()) {
        return it->second[direction_]->getFrame(currentIdx_).texture_;
    } else {
        boost::shared_ptr<ui::Texture> ret;
        return ret;
    }
}

AnimationFrame AnimTextureProvider::getCurrentFrame() const {
    std::map<unsigned int, std::vector<boost::shared_ptr<Animation> > >::const_iterator it = animations_.find(currentAnimId_);
    if (it != animations_.end() && it->second[direction_]->isReadComplete()) {
        return it->second[direction_]->getFrame(currentIdx_);
    } else {
        it = animations_.find(defaultAnimId_);
        if (it != animations_.end() && it->second[direction_]->isReadComplete()) {
            return it->second[direction_]->getFrame(currentIdx_);
        } else {
            return AnimationFrame();
        }
    }
}

void AnimTextureProvider::setAnimId(unsigned int animId) {
    if (animId != currentAnimId_) {
        nextAnimId_ = animId;
    }
}

bool AnimTextureProvider::update(unsigned int elapsedMillis) {
    bool frameChanged = false;
    std::map<unsigned int, std::vector<boost::shared_ptr<Animation> > >::const_iterator it;
    if (nextAnimId_ != 0xFFFFFFFFu) {
        // change of anim id requested
        it = animations_.find(nextAnimId_);

        if (it == animations_.end()) {
            animations_[nextAnimId_] = data::Manager::getAnim(bodyId_, nextAnimId_);
            it = animations_.find(nextAnimId_);
        }

        if (!it->second[direction_]) {
            //LOG_DEBUG << "Request for invalid anim id " << nextAnimId_ << " on body id " << bodyId_ << std::endl;
        } else if (it->second[direction_]->isReadComplete()) {
            currentAnimId_ = nextAnimId_;
            currentIdx_ = 0;
            millis_ = 0;
            elapsedMillis = 0;

            frameChanged = true;
            nextAnimId_ = 0xFFFFFFFFu;
        }
    }

    if (nextDirection_ != direction_) {
        frameChanged = true;
        direction_ = nextDirection_;
    }

    it = animations_.find(currentAnimId_);
    if (it == animations_.end() || animations_.size() == 0 || it->second.empty() || !it->second[direction_]->isReadComplete() || elapsedMillis == 0) {
        return frameChanged;
    }

    unsigned int lastIdx = currentIdx_;

    unsigned int newMillis = millis_ + elapsedMillis;

    unsigned int maxMillis = it->second[direction_]->getFrameCount() * frameMillis_;
    if (haltMillis_ > 0) {
        if (haltMillis_ <= elapsedMillis) {
            // halt ended with nothing happening
            haltMillis_ = 0;
            activateDefaultAnim();
        } else {
            haltMillis_ -= elapsedMillis;
        }
        // don't count the elapsed time in this case
        newMillis = millis_;

    } else if (newMillis >= maxMillis) {
        switch (repeatMode_) {
            case AnimRepeatMode::LOOP:
                currentIdx_ = 0;
                newMillis %= maxMillis;
                break;
            case AnimRepeatMode::LAST:
                // we are already at the last frame here
                break;
            case AnimRepeatMode::DEFAULT:
                activateDefaultAnim();
                //frameChanged = true;
                break;
        }
    } else {
        currentIdx_ = newMillis / frameMillis_;
    }

    millis_ = newMillis;

    frameChanged |= lastIdx != currentIdx_;

    return frameChanged;
}

void AnimTextureProvider::setDirection(unsigned int direction) {
    direction &= 0x7; // only last 3 bits are interesting
    nextDirection_ = direction;
}

void AnimTextureProvider::setRepeatMode(unsigned int mode) {
    repeatMode_ = mode;
}

void AnimTextureProvider::activateDefaultAnim() {
    setAnimId(defaultAnimId_);
}

void AnimTextureProvider::setDelay(unsigned int delay) {
    frameMillis_ = 100 + delay * 50;
}

void AnimTextureProvider::setDefaultAnimId(unsigned int animId) {
    bool changeCurrent = currentAnimId_ == defaultAnimId_;
    defaultAnimId_ = animId;

    if (changeCurrent) {
        activateDefaultAnim();
    }
}

unsigned int AnimTextureProvider::getDefaultAnimId() const {
    return defaultAnimId_;
}

bool AnimTextureProvider::isHalted() const {
    return haltMillis_ > 0;
}

void AnimTextureProvider::halt() {
    haltMillis_ = 1000;
}

void AnimTextureProvider::resume() {
    haltMillis_ = 0;
}

}
}
