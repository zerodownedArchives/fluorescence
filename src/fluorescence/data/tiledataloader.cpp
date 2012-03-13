/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "tiledataloader.hpp"

#include "fullfileloader.hpp"

#include <misc/log.hpp>

#include <boost/bind.hpp>

namespace fluo {
namespace data {

TileDataLoader::TileDataLoader(const boost::filesystem::path& path, bool highSeasFormat) : highSeasFormat_(highSeasFormat) {
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

const LandTileInfo* TileDataLoader::getLandTileInfo(unsigned int id) {
    if (id > landTileCount_) {
        LOG_WARN << "Trying to access out of range land tile data index " << id << std::endl;
        id = 0;
    }

    return &landTileInfos_[id];
}

const StaticTileInfo* TileDataLoader::getStaticTileInfo(unsigned int id) {
    if (id > staticTileCount_) {
        LOG_WARN << "Trying to access out of range static tile data index " << id << std::endl;
        id = 0;
    }

    return &staticTileInfos_[id];
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
        ptr += highSeasFormat_ ? 8 : 4;
        landTileInfos_[i].textureId_ = *(reinterpret_cast<uint16_t*>(ptr));
        ptr += 2;
        landTileInfos_[i].name_ = StringConverter::fromUtf8(reinterpret_cast<char*>(ptr), 20);
        ptr += 20;
    }

    // calculate number of static tile blocks

    if (highSeasFormat_) {
        staticTileCount_ = ((len - (ptr - buf)) / 1316) * 32;
    } else {
        staticTileCount_ = ((len - (ptr - buf)) / 1188) * 32;
    }
    
    LOG_DEBUG << "Static tile count: " << staticTileCount_ << std::endl;

    staticTileInfos_ = new StaticTileInfo[staticTileCount_];

    for (unsigned int i = 0; i < staticTileCount_; ++i) {
        // jump header
        if ((i % 32) == 0) {
            ptr += 4;
        }

        staticTileInfos_[i].flags_ = *(reinterpret_cast<uint32_t*>(ptr));
        ptr += highSeasFormat_ ? 8 : 4;

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

    LOG_DEBUG << "Total read bytes: " << (ptr-buf) << " len: " << len << std::endl;

}


}
}
