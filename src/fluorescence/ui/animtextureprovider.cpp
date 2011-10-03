
#include "animtextureprovider.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/mobtypesloader.hpp>

namespace fluo {
namespace ui {

AnimTextureProvider::AnimTextureProvider(unsigned int bodyId) : bodyId_(bodyId), direction_(0), currentIdx_(0),
        millis_(0), frameMillis_(150), repeatMode_(REPEAT_MODE_DEFAULT), nextAnimId_(0xFFFFFFFFu) {
    defaultAnimId_ = data::Manager::getMobTypesLoader()->getIdleAction(bodyId);
    currentAnimId_ = defaultAnimId_;

    animations_[defaultAnimId_] = data::Manager::getAnim(bodyId, defaultAnimId_);
}

boost::shared_ptr<ui::Texture> AnimTextureProvider::getTexture() const {
    std::map<unsigned int, std::vector<boost::shared_ptr<Animation> > >::const_iterator it = animations_.find(currentAnimId_);
    if (it != animations_.end() && it->second[direction_]->isReadComplete()) {
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
        }

        it = animations_.find(nextAnimId_);

        if (!it->second.empty() && it->second[direction_]->isReadComplete()) {
            currentAnimId_ = nextAnimId_;
            currentIdx_ = 0;
            millis_ = 0;
            elapsedMillis = 0;

            nextAnimId_ = 0xFFFFFFFFu;

            frameChanged = true;
        }
    }

    it = animations_.find(currentAnimId_);
    if (it == animations_.end() || animations_.size() == 0 || it->second.empty() || !it->second[direction_]->isReadComplete() || elapsedMillis == 0) {
        return frameChanged;
    }

    unsigned int lastIdx = currentIdx_;

    unsigned int newMillis = millis_ + elapsedMillis;

    unsigned int maxMillis = it->second[direction_]->getFrameCount() * frameMillis_;
    if (newMillis >= maxMillis) {
        switch (repeatMode_) {
            case REPEAT_MODE_LOOP:
                currentIdx_ = 0;
                newMillis %= maxMillis;
                break;
            case REPEAT_MODE_LAST:
                // we are already at the last frame here
                break;
            case REPEAT_MODE_DEFAULT:
                setAnimId(defaultAnimId_);
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
    direction_ = direction;
}

}
}
