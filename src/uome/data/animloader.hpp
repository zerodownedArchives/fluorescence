#ifndef UOME_DATA_ANIMLOADER_HPP
#define UOME_DATA_ANIMLOADER_HPP

#include "weakptrcache.hpp"
#include "indexedondemandfileloader.hpp"

#include <boost/filesystem.hpp>

namespace uome {

namespace ui {
    class Animation;
}

namespace data {

class AnimLoader {
public:
    AnimLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath, unsigned int highDetailCount, unsigned int lowDetailCount);

    boost::shared_ptr<ui::Animation> getAnimation(unsigned int bodyId, unsigned int animId, unsigned int direction);

    void readCallback(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<ui::Animation> anim, unsigned int extra, unsigned int userData);

private:
    unsigned int highDetailCount_;
    unsigned int lowDetailCount_;
    WeakPtrCache<ui::Animation, IndexedOnDemandFileLoader> cache_;
};

}
}

#endif
