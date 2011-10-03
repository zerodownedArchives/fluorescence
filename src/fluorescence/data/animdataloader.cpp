
#include "animdataloader.hpp"

#include <boost/bind.hpp>

#include "fullfileloader.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace data {

AnimDataLoader::AnimDataLoader(const boost::filesystem::path& path) {
    FullFileLoader loader(path);
    loader.read(boost::bind(&AnimDataLoader::readCallback, this, _1, _2));
}

void AnimDataLoader::readCallback(int8_t* buf, unsigned int len) {
    unsigned int blockCount = (len / ((68 * 8) + 4));
    unsigned int itemCount = blockCount * 8;

    int8_t* ptr = buf;

    for (unsigned int artIdx = 0; artIdx < itemCount; ++artIdx) {
        if (artIdx % 8 == 0) {
            // jump header
            ptr += 4;
        }

        AnimDataInfo curInfo;

        uint8_t frameCount = *(reinterpret_cast<uint8_t*>(ptr + 65));
        curInfo.frameIntervalMillis_ = *(reinterpret_cast<uint8_t*>(ptr + 66));
        curInfo.frameStart_ = *(reinterpret_cast<uint8_t*>(ptr + 67));

        curInfo.frameStart_ *= 50;
        curInfo.frameIntervalMillis_ *= 50;
        curInfo.frameCount_ = frameCount;

        for (unsigned int frame = 0; frame < 64; ++frame) {
            curInfo.artIds_[frame] = artIdx + *(ptr + frame);
        }

        if (frameCount > 0) {
            //LOGARG_DEBUG(LOGTYPE_DATA, "Added animdata id %#x", artIdx);
            infos_[artIdx] = curInfo;
        }

        ptr += 68;
    }

    //LOGARG_INFO(LOGTYPE_DATA, "AnimDataLoader count: %#x", itemCount);
}

AnimDataInfo AnimDataLoader::getInfo(unsigned int artId) {
    std::map<unsigned int, AnimDataInfo>::iterator iter = infos_.find(artId);
    if (iter != infos_.end()) {
        return infos_[artId];
    } else {
        AnimDataInfo ret;
        return ret;
    }
}

}
}
