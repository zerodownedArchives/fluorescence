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



#include "maptexloader.hpp"
#include "util.hpp"

#include <ui/texture.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace data {

MapTexLoader::MapTexLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath) {

    boost::shared_ptr<IndexedOnDemandFileLoader<unsigned int, ui::Texture> > loader(new IndexedOnDemandFileLoader<unsigned int, ui::Texture>(idxPath, mulPath,
            boost::bind(&MapTexLoader::readCallback, this, _1, _2, _3, _4, _5, _6)));
    cache_.init(loader);
}

boost::shared_ptr<ui::Texture> MapTexLoader::get(unsigned int id) {
    return cache_.get(id);
}

void MapTexLoader::readCallback(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<ui::Texture> tex, unsigned int extra, unsigned int userData) {
    tex->setUsage(ui::Texture::USAGE_WORLD);
    tex->setBorderWidth(1);
    
    // map textures are always quadratic
    unsigned short width = (extra == 1) ? 128 : 64;

    tex->initPixelBuffer(width, width);
    uint32_t* pixBufPtr = tex->getPixelBufferData();

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
