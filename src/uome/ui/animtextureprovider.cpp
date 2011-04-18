
#include "animtextureprovider.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>

namespace uome {
namespace ui {

AnimTextureProvider::AnimTextureProvider(unsigned int animId) : currentAnimId_(0), currentIdx_(0), millis_(0), frameMillis_(0) {
    animations_ = data::Manager::getFullAnim(animId);

    if (animations_.size() == 0) {
        LOGARG_WARN(LOGTYPE_DATA, "Loading unknown animId: %u", animId);
        AnimationFrame frame;
        frame.texture_ = data::Manager::getArtLoader()->getItemTexture(1);

        boost::shared_ptr<Animation> anim(new Animation());
        anim->addFrame(frame);

        animations_.push_back(anim);
    }
}

boost::shared_ptr<ui::Texture> AnimTextureProvider::getTexture() {
    boost::shared_ptr<ui::Texture> ret = animations_[currentAnimId_]->getFrame(currentIdx_).texture_;
    return ret;
}

void AnimTextureProvider::update(unsigned int elapsedMillis) {
    unsigned int newMillis = millis_ + elapsedMillis;

    unsigned int maxMillis = animations_[currentAnimId_]->getFrameCount() * frameMillis_;
    if (newMillis >= maxMillis) {
        // TODO: different end behaviors (stop at end, loop, goto first
        currentIdx_ = 0;
        newMillis %= maxMillis;
    } else {
        currentIdx_ = newMillis / frameMillis_;
    }

    millis_ = newMillis;
}

}
}
