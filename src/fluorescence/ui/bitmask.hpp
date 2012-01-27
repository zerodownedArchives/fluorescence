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


#ifndef FLUO_UI_BITMASK_HPP
#define FLUO_UI_BITMASK_HPP

#include <boost/shared_ptr.hpp>
#include <ClanLib/Display/Image/pixel_buffer.h>

#include <stdint.h>

namespace fluo {
namespace ui {

/// This class is used to store pixel visibility for a texture
class BitMask {
public:
    BitMask();
    ~BitMask();

    void init(boost::shared_ptr<CL_PixelBuffer> pixBuf);
    void init(const CL_PixelBuffer& pixBuf);

    bool hasPixel(unsigned int pixelX, unsigned int pixelY);

private:
    unsigned int width_;
    unsigned int height_;

    unsigned int bitStoreSize_;
    uint8_t* bitStore_;

    void setPixel(unsigned int pixelX, unsigned int pixelY);
};

}
}

#endif
