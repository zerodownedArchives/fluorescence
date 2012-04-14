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



#include "artloader.hpp"
#include "util.hpp"

#include <ui/texture.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace data {

ArtLoader::ArtLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath) {

    boost::shared_ptr<IndexedOnDemandFileLoader<unsigned int, ui::Texture> > loader(new IndexedOnDemandFileLoader<unsigned int, ui::Texture>(idxPath, mulPath,
                boost::bind(&ArtLoader::readCallback, this, _1, _2, _3, _4, _5, _6)));
    cache_.init(loader);

    // fix default graphics
    cache_.fixItem(0);
    cache_.fixItem(0x4000);
}

boost::shared_ptr<ui::Texture> ArtLoader::getMapTexture(unsigned int id) {
    if (id > 0x4000) {
        LOG_WARN << "Trying to load too high map tile texture " << id << std::endl;
        id = 0;
    }

    return cache_.get(id);
}

boost::shared_ptr<ui::Texture> ArtLoader::getItemTexture(unsigned int id) {
    id += 0x4000;

    return cache_.get(id);
}

void ArtLoader::readCallback(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<ui::Texture> tex, unsigned int extra, unsigned int userData) {
    tex->setUsage(ui::Texture::USAGE_WORLD);
    
    unsigned short height = 44;
    unsigned short width = 44;

    uint16_t* inputPtr = reinterpret_cast<uint16_t*>(buf);

    if (index >= 0x4000) {
        // 4 unknown bytes
        inputPtr += 2;

        width = *inputPtr;
        ++inputPtr;
        height = *inputPtr;
        ++inputPtr;
    }

    tex->initPixelBuffer(width, height);
    uint32_t* pixBufPtr = tex->getPixelBufferData();

    unsigned int curX = 22;
    unsigned int curY = 0;
    unsigned int lineWidth = 0;

    if (index < 0x4000) {
        // map tile
        for (unsigned int i = 0; i < 22; ++i) {
            --curX;
            lineWidth += 2;
            //LOGARG_DEBUG(LOGTYPE_DATA, "zeile y=%u x=%u width=%u", curY, curX, lineWidth);
            for (unsigned int i = 0; i < lineWidth; i++) {
                uint32_t pixel = Util::getColorRGBA(*inputPtr);
                ++inputPtr;
                pixBufPtr[(curY * 44) + curX + i] = pixel;
            }
            ++curY;
        }

        for (unsigned int i = 0; i < 22; ++i) {
            //LOGARG_DEBUG(LOGTYPE_DATA, "zeile y=%u x=%u width=%u", curY, curX, lineWidth);
            for (unsigned int i = 0; i < lineWidth; i++) {
                uint32_t pixel = Util::getColorRGBA(*inputPtr);
                ++inputPtr;
                pixBufPtr[(curY * 44) + curX + i] = pixel;
            }
            ++curY;
            ++curX;
            lineWidth -= 2;
        }

    } else {
        uint16_t* lookupList = inputPtr;
        inputPtr += height;
        uint16_t* datastart = inputPtr;
        for (uint32_t y = 0; y < height; y++) {
            inputPtr = datastart + lookupList[y];

            uint32_t x = 0;
            uint16_t xOffset = *inputPtr++;
            uint16_t runLength = *inputPtr++;

            while (runLength) {
                x += xOffset;

                while (runLength) {
                    uint16_t color = *inputPtr++;

                    pixBufPtr[(y * width) + x] = Util::getColorRGBA(color);
                    x++;
                    runLength--;
                }

                xOffset = *inputPtr++;
                runLength = *inputPtr++;
            }
        }
    }
}

void ArtLoader::printStats() {
    LOG_DEBUG << "ArtLoader stats:" << std::endl;
    cache_.printStats();
}

}
}
