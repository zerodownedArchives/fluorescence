
#include "animtextureprovider.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>

namespace uome {
namespace ui {

AnimTextureProvider::AnimTextureProvider(unsigned int animId) : currentAnimId_(0), currentIdx_(0), millis_(0), frameMillis_(100), repeatMode_(REPEAT_MODE_LOOP) {
    animations_ = data::Manager::getFullAnim(animId);

    if (animations_.size() == 0) {
        LOG_WARN << "Loading unknown animId: " << animId << "  " << std::hex << animId << std::dec << std::endl;
        AnimationFrame frame;
        frame.texture_ = data::Manager::getArtLoader()->getItemTexture(1);

        boost::shared_ptr<Animation> anim(new Animation());
        anim->addFrame(frame);

        animations_.push_back(anim);
    }
}

boost::shared_ptr<ui::Texture> AnimTextureProvider::getTexture() const {
    boost::shared_ptr<ui::Texture> ret = animations_[currentAnimId_]->getFrame(currentIdx_).texture_;
    return ret;
}

AnimationFrame AnimTextureProvider::getCurrentFrame() const {
    return animations_[currentAnimId_]->getFrame(currentIdx_);
}

bool AnimTextureProvider::update(unsigned int elapsedMillis) {
    if (animations_.size() == 0 || !animations_[currentAnimId_]->isReadComplete() || elapsedMillis == 0) {
        return false;
    }

    unsigned int lastIdx = currentIdx_;

    unsigned int newMillis = millis_ + elapsedMillis;

    unsigned int maxMillis = animations_[currentAnimId_]->getFrameCount() * frameMillis_;
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
                // TODO: use meaningful default here
                currentIdx_ = 4;
                break;
        }
    } else {
        currentIdx_ = newMillis / frameMillis_;
    }

    millis_ = newMillis;

    return lastIdx != currentIdx_;
}

}
}
