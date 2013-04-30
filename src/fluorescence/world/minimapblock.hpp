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


#ifndef FLUO_WORLD_MINIMAPBLOCK_HPP
#define FLUO_WORLD_MINIMAPBLOCK_HPP

#include <boost/shared_ptr.hpp>
#include <ClanLib/Display/Image/pixel_buffer.h>

#include <typedefs.hpp>

namespace fluo {

namespace ui {
class Texture;
}

namespace world {

class Sector;

class MiniMapBlock {
public:
    static const unsigned int SECTOR_ID_MODULO = 8;

    MiniMapBlock(const IsoIndex& topLeft);

    const IsoIndex& getTopLeftIndex() const;

    void updateSector(world::Sector* sector);

    boost::shared_ptr<ui::Texture> getTexture();
    const CL_Vec3f* getVertexCoordinates() const;

private:
    IsoIndex topLeftIndex_;

    CL_PixelBuffer pixelBuffer_;
    boost::shared_ptr<ui::Texture> texture_;

    CL_Vec3f vertexCoordinates_[6];
};

}
}

#endif

