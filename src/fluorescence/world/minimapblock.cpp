/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2010-2013, http://fluorescence-client.org

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

#include "minimapblock.hpp"

#include "sector.hpp"

#include <list>

#include <data/manager.hpp>
#include <data/radarcolloader.hpp>
#include <ui/texture.hpp>
#include <world/ingameobject.hpp>
#include <world/map.hpp>

namespace fluo {
namespace world {

MiniMapBlock::MiniMapBlock(const IsoIndex& topLeft) : topLeftIndex_(topLeft), pixelBuffer_(SECTOR_ID_MODULO * 8, SECTOR_ID_MODULO * 8, cl_rgba8) {
    CL_Rectf rect(topLeftIndex_.x_ * 8, topLeftIndex_.y_ * 8, CL_Sizef(SECTOR_ID_MODULO * 8, SECTOR_ID_MODULO * 8));
    vertexCoordinates_[0].x = rect.left;
    vertexCoordinates_[0].y = rect.top;
    vertexCoordinates_[1].x = rect.right;
    vertexCoordinates_[1].y = rect.top;
    vertexCoordinates_[2].x = rect.left;
    vertexCoordinates_[2].y = rect.bottom;
    vertexCoordinates_[3].x = rect.right;
    vertexCoordinates_[3].y = rect.top;
    vertexCoordinates_[4].x = rect.left;
    vertexCoordinates_[4].y = rect.bottom;
    vertexCoordinates_[5].x = rect.right;
    vertexCoordinates_[5].y = rect.bottom;
}

void MiniMapBlock::updateSector(world::Sector* sector) {
    unsigned int pixelOffsetX = (sector->getSectorId().x_ % SECTOR_ID_MODULO) * 8;
    unsigned int pixelOffsetY = (sector->getSectorId().y_ % SECTOR_ID_MODULO) * 8;

    unsigned int rowLength = SECTOR_ID_MODULO * 8;

    // point to the start of this sector's data
    uint32_t* pxBufPtr = pixelBuffer_.get_data_uint32();
    pxBufPtr += pixelOffsetY * rowLength;
    pxBufPtr += pixelOffsetX;

    const uint32_t* staticPixels = sector->getStaticMiniMapPixels();
    const int8_t* staticHeights = sector->getStaticMiniMapHeight();
    const uint32_t* mapPixels = sector->getMapMiniMapPixels();
    const int8_t * mapHeights = sector->getMapMiniMapHeight();

    if (!staticPixels) {
        // no statics on this block, just copy map
        for (unsigned int i = 0; i < 8; ++i) {
            memcpy(pxBufPtr, mapPixels + (i * 8), 8 * 4);
            pxBufPtr += rowLength;
        }
    } else {
        for (unsigned int i = 0; i < 8; ++i) {
            for (unsigned int j = 0; j < 8; ++j) {
                unsigned int idx = i * 8 + j;
                pxBufPtr[j] = mapHeights[idx] > staticHeights[idx] ? mapPixels[idx] : staticPixels[idx];
            }
            pxBufPtr += rowLength;
        }
    }

    texture_.reset();
}

boost::shared_ptr<ui::Texture> MiniMapBlock::getTexture() {
    if (!texture_) {
        texture_.reset(new ui::Texture(ui::Texture::USAGE_MINIMAP, false));
        texture_->setBorderWidth(1);
        texture_->setTexture(pixelBuffer_);
    }

    return texture_;
}

const IsoIndex& MiniMapBlock::getTopLeftIndex() const {
    return topLeftIndex_;
}

const CL_Vec3f* MiniMapBlock::getVertexCoordinates() const {
    return vertexCoordinates_;
}

}
}
