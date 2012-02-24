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


#ifndef FLUO_UI_TEXTURE_HPP
#define FLUO_UI_TEXTURE_HPP

#include <ClanLib/Display/Image/pixel_buffer.h>
#include <ClanLib/Display/Image/texture_format.h>
#include <ClanLib/Display/Render/texture.h>

#include <boost/shared_ptr.hpp>

#include "bitmask.hpp"
#include <data/ondemandreadable.hpp>

namespace fluo {
namespace ui {

class Texture : public fluo::data::OnDemandReadable<Texture> {
public:
    Texture(bool useBitMask = true);

    void initPixelBuffer(unsigned int width, unsigned int height);
    uint32_t* getPixelBufferData();

    boost::shared_ptr<CL_PixelBuffer> getPixelBuffer();

    boost::shared_ptr<CL_Texture> getTexture();
    void setTexture(CL_PixelBuffer& pixBuf);

    float getWidth();
    float getHeight();

    bool hasPixel(unsigned int pixelX, unsigned int pixelY);

private:
    boost::shared_ptr<CL_PixelBuffer> pixelBuffer_;
    boost::shared_ptr<CL_Texture> texture_;
    bool useBitMask_;
    BitMask bitMask_;
};
}
}

#endif
