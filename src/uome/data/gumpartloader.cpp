
#include "gumpartloader.hpp"
#include "util.hpp"

#include <ui/texture.hpp>

#include <misc/log.hpp>

namespace uome {
namespace data {

GumpArtLoader::GumpArtLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath) {

    boost::shared_ptr<IndexedOnDemandFileLoader<ui::Texture> > loader(new IndexedOnDemandFileLoader<ui::Texture>(idxPath, mulPath,
                boost::bind(&GumpArtLoader::readCallback, this, _1, _2, _3, _4, _5, _6)));
    cache_.init(loader);
}

boost::shared_ptr<ui::Texture> GumpArtLoader::getTexture(unsigned int id) {
    return cache_.get(id);
}

void GumpArtLoader::readCallback(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<ui::Texture> tex, unsigned int extra, unsigned int userData) {
    unsigned int width = (extra >> 16) & 0xFFFF;
    unsigned int height = extra & 0xFFFF;

    tex->initPixelBuffer(width, height);
    uint32_t* pixBufPtr = tex->getPixelBufferData();
    memset(pixBufPtr, 0, width * height * sizeof(uint32_t));


    // LOG_INFO("start decode loop", etc::LOGTYPE_DATA);
    uint32_t* lookupList = reinterpret_cast<uint32_t*>(buf);
    int8_t* start = buf;

    for(unsigned int y = 0; y < height; ++y) {
        uint32_t size;
        if (y < height-1) {
            size = lookupList[y+1] - lookupList[y];
        } else {
            size = (len / 4) - lookupList[y];
        }

        unsigned int x = 0;

        uint16_t* value = reinterpret_cast<uint16_t*>(start + lookupList[y]*4);
        uint16_t* run = value + 1;
        uint32_t color = Util::getColorRGBA(*value);

        for(unsigned int i = 0; i < size; i++) {
            if (*value > 0) {
                for(unsigned int j = 0; j < *run; j++) {
                    pixBufPtr[(y * width) + x] = color;
                    x++;
                }
            } else {
                x += *run;
            }

            value += 2;
            run += 2;
            color = Util::getColorRGBA(*value);
        }
    }
}

}
}
