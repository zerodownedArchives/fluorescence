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
#include <ClanLib/Display/2D/subtexture.h>

#include "bitmask.hpp"
#include <data/ondemandreadable.hpp>

namespace fluo {
namespace ui {

class Texture : public fluo::data::OnDemandReadable<Texture> {
public:
    enum Usage {
        USAGE_WORLD = 0,
        USAGE_GUMP = 1,
        USAGE_FONT = 2,
        USAGE_EFFECT = 3,
    };

    Texture(bool useBitMask = true);
    Texture(Usage usage, bool useBitMask = true);
    ~Texture();

    void setUsage(unsigned int usage);

    void initPixelBuffer(unsigned int width, unsigned int height);
    uint32_t* getPixelBufferData();

    CL_PixelBuffer getPixelBuffer();

    CL_Texture getTexture();
    void setTexture(const CL_PixelBuffer& pixBuf);
    CL_Rectf getTextureCoords();
    CL_Rectf getNormalizedTextureCoords();

    float getWidth();
    float getHeight();

    bool hasPixel(unsigned int pixelX, unsigned int pixelY);

    // very expensive operation, if the CL_Subtexture object was already initialized. use with care
    CL_Texture extractSingleTexture();

    void setBorderWidth(unsigned int width);

    void debugSaveToFile(const char* filename);

private:
    CL_PixelBuffer pixelBuffer_;
    CL_Subtexture texture_;
    bool useBitMask_;
    BitMask bitMask_;
    unsigned int textureUsage_;
    CL_Rectf normalizedTextureCoords_;

    void initSubTexture();

    unsigned int borderWidth_;
    CL_Rectf borderlessGeometry_;
};
}
}

#endif
