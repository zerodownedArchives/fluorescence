#ifndef UOME_DATA_ANIMDATALOADER_HPP
#define UOME_DATA_ANIMDATALOADER_HPP

#include <list>
#include <map>

#include <boost/filesystem/path.hpp>

namespace uome {
namespace data {

struct AnimDataInfo {
    unsigned int artIds_[64];
    unsigned int frameCount_;
    unsigned int frameIntervalMillis_;
    unsigned int frameStart_;
};

class AnimDataLoader {
public:
    AnimDataLoader(const boost::filesystem::path& path);

    void readCallback(int8_t* buf, unsigned int len);

    AnimDataInfo getInfo(unsigned int artId);

private:
    std::map<unsigned int, AnimDataInfo> infos_;

};

}
}

#endif
