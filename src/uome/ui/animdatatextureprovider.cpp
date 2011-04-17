
#include "animdatatextureprovider.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>

namespace uome {
namespace ui {

AnimDataTextureProvider::AnimDataTextureProvider(unsigned int artId) : currentIdx_(0), millis_(0) {
    info_ = data::Manager::getAnimDataLoader()->getInfo(artId);

    LOGARG_WARN(LOGTYPE_DATA, "Using AnimDataTexPro for artId=%u frameCount=%u", artId, info_.frameCount_);

    if (info_.frameCount_ == 0) {
        LOGARG_WARN(LOGTYPE_DATA, "Using artId withouth animdata for animation: %u", artId);
        boost::shared_ptr<ui::Texture> tex = data::Manager::getArtLoader()->getItemTexture(1);
        textures_.push_back(tex);
    }

    for (unsigned int i = 0; i < info_.frameCount_; ++i) {
        LOGARG_WARN(LOGTYPE_DATA, "\t: anim frame: %u", info_.artIds_[i]);
        boost::shared_ptr<ui::Texture> tex = data::Manager::getArtLoader()->getItemTexture(info_.artIds_[i]);
        textures_.push_back(tex);
    }
}

boost::shared_ptr<ui::Texture> AnimDataTextureProvider::getTexture() {
    boost::shared_ptr<ui::Texture> ret = textures_[currentIdx_];
    return ret;
}

void AnimDataTextureProvider::update(unsigned int elapsedMillis) {
    unsigned int newMillis = millis_ + elapsedMillis;

    unsigned int maxMillis = info_.frameStart_ + info_.frameCount_ * info_.frameIntervalMillis_;
    if (newMillis >= maxMillis) {
        currentIdx_ = 0;
        newMillis -= maxMillis;
    } else if (newMillis <= info_.frameStart_) {
        currentIdx_ = 0;
    } else {
        currentIdx_ = (newMillis - info_.frameStart_) / info_.frameIntervalMillis_;
    }

    millis_ = newMillis;
}

}
}
