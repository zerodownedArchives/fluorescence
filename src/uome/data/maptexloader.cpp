
#include "maptexloader.hpp"
#include "util.hpp"

#include <ui/texture.hpp>

#include <misc/log.hpp>

namespace uome {
namespace data {

MapTexLoader::MapTexLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath) {

    boost::shared_ptr<IndexedOnDemandFileLoader<ui::Texture> > loader(new IndexedOnDemandFileLoader<ui::Texture>(idxPath, mulPath,
            boost::bind(&MapTexLoader::readCallback, this, _1, _2, _3, _4, _5, _6)));
    cache_.init(loader);
}

boost::shared_ptr<ui::Texture> MapTexLoader::get(unsigned int id) {
    return cache_.get(id);
}

void MapTexLoader::readCallback(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<ui::Texture> tex, unsigned int extra, unsigned int userData) {
    // map textures are always quadratic
    unsigned short width = (extra == 1) ? 128 : 64;

    tex->initPixelBuffer(width, width);
    uint32_t* pixBufPtr = tex->getPixelBufferData();
    memset(pixBufPtr, 0, width * width * sizeof(uint32_t));

    uint16_t* inputPtr = reinterpret_cast<uint16_t*>(buf);

    for (unsigned int x = 0; x < width; ++x) {
        for (unsigned int y = 0; y < width; ++y) {
            uint32_t pixel = Util::getColorRGBA(*inputPtr);
            ++inputPtr;
            pixBufPtr[(y * width) + x] = pixel;
        }
    }
}

}
}
