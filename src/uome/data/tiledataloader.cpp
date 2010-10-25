
#include "tiledataloader.hpp"

#include "fullfileloader.hpp"

#include <boost/bind.hpp>

namespace uome {
namespace data {

TileDataLoader::TileDataLoader(const boost::filesystem::path& path) {
    FullFileLoader ldr(path);
    ldr.read(boost::bind(&TileDataLoader::read, this, _1, _2));
}

TileDataLoader::~TileDataLoader() {
    if (landTileInfos_) {
        delete [] landTileInfos_;
        landTileInfos_ = 0;
    }

    if (staticTileInfos_) {
        delete [] staticTileInfos_;
        staticTileCount_ = 0;
    }
}

const LandTileInfo& TileDataLoader::getLandTileInfo(unsigned int id) {
    if (id > landTileCount_) {
        LOGARG_WARN(LOGTYPE_DATA, "Trying to access out of range land tile data index %u", id);
        id = 0;
    }

    return landTileInfos_[id];
}

const StaticTileInfo& TileDataLoader::getStaticTileInfo(unsigned int id) {
    if (id > staticTileCount_) {
        LOGARG_WARN(LOGTYPE_DATA, "Trying to access out of range static tile data index %u", id);
        id = 0;
    }

    return staticTileInfos_[id];
}

void TileDataLoader::read(int8_t* buf, unsigned int len) {
    landTileCount_ = LAND_TILE_COUNT;

    landTileInfos_ = new LandTileInfo[landTileCount_];

    int8_t* ptr = buf;

    for (unsigned int i = 0; i < landTileCount_; ++i) {
        // jump header
        if ((i % 32) == 0) {
            ptr += 4;
        }

        landTileInfos_[i].flags_ = *(reinterpret_cast<uint32_t*>(ptr));
        ptr += 4;
        landTileInfos_[i].textureId_ = *(reinterpret_cast<uint16_t*>(buf + 4));
        ptr += 2;
        landTileInfos_[i].name_ = StringConverter::fromUtf8(reinterpret_cast<char*>(buf + 6), 20);
        ptr += 20;
    }

    // calculate number of static tile blocks

    staticTileCount_ = ((len - (ptr - buf)) / 1188) * 32;
    LOGARG_DEBUG(LOGTYPE_DATA, "Static tile count: %u", staticTileCount_);

    staticTileInfos_ = new StaticTileInfo[staticTileCount_];

    for (unsigned int i = 0; i < staticTileCount_; ++i) {
        // jump header
        if ((i % 32) == 0) {
            ptr += 4;
        }

        staticTileInfos_[i].flags_ = *(reinterpret_cast<uint32_t*>(ptr));
        ptr += 4;

        staticTileInfos_[i].weight_ = *(reinterpret_cast<uint8_t*>(ptr));
        ++ptr;

        staticTileInfos_[i].quality_ = *(reinterpret_cast<uint8_t*>(ptr));
        ++ptr;

        staticTileInfos_[i].unknown1_ = *(reinterpret_cast<uint16_t*>(ptr));
        ptr += 2;

        staticTileInfos_[i].unknown2_ = *(reinterpret_cast<uint8_t*>(ptr));
        ++ptr;

        staticTileInfos_[i].quantity_ = *(reinterpret_cast<uint8_t*>(ptr));
        ++ptr;

        staticTileInfos_[i].animId_ = *(reinterpret_cast<uint16_t*>(ptr));
        ptr += 2;

        staticTileInfos_[i].unknown3_ = *(reinterpret_cast<uint8_t*>(ptr));
        ++ptr;

        staticTileInfos_[i].hue_ = *(reinterpret_cast<uint8_t*>(ptr));
        ++ptr;

        staticTileInfos_[i].unknown4_ = *(reinterpret_cast<uint8_t*>(ptr));
        ++ptr;

        staticTileInfos_[i].unknown5_ = *(reinterpret_cast<uint8_t*>(ptr));
        ++ptr;

        staticTileInfos_[i].height_ = *(reinterpret_cast<uint8_t*>(ptr));
        ++ptr;

        staticTileInfos_[i].name_ = StringConverter::fromUtf8(reinterpret_cast<char*>(ptr), 20);
        ptr += 20;
    }

    LOGARG_DEBUG(LOGTYPE_DATA, "Total read bytes: %u, len: %u", (ptr - buf), len);

}


}
}
