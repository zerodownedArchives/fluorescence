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



#include "texture.hpp"

#include "manager.hpp"
#include "bitmask.hpp"

#include <misc/exception.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace ui {

Texture::Texture(bool useBitMask) : useBitMask_(useBitMask), textureUsage_(0xFFFFFFFF), borderWidth_(0) {
}

Texture::Texture(Usage usage, bool useBitMask) : useBitMask_(useBitMask), textureUsage_(usage), borderWidth_(0) {
}

Texture::~Texture() {
    if (!texture_.is_null()) {
        ui::Manager::getSingleton()->freeTexture(textureUsage_, texture_);
    }
}

void Texture::setUsage(unsigned int usage) {
    if (textureUsage_ == 0xFFFFFFFF) {
        textureUsage_ = usage;
    }
}

void Texture::initPixelBuffer(unsigned int width, unsigned int height) {
    pixelBuffer_ = CL_PixelBuffer(width, height, cl_rgba8);

    memset(getPixelBufferData(), 0, width * height * sizeof(uint32_t));
}

uint32_t* Texture::getPixelBufferData() {
    return reinterpret_cast<uint32_t*>(pixelBuffer_.get_data());
}

CL_PixelBuffer Texture::getPixelBuffer() {
    return pixelBuffer_;
}

CL_Texture Texture::getTexture() {
    if (textureUsage_ == 0xFFFFFFFF) {
        LOG_EMERGENCY << "Creating texture without specified usage" << std::endl;
        throw Exception("Unable to create texture without specified usage");
    }

    if (texture_.is_null()) {
        initSubTexture();
    }

    return texture_.get_texture();
}

void Texture::setTexture(const CL_PixelBuffer& pixBuf) {
    if (texture_.is_null()) {
        pixelBuffer_ = pixBuf.copy();

        if (useBitMask_) {
            bitMask_.init(pixelBuffer_);
        }

        setReadComplete();
    }
}

float Texture::getWidth() {
    if (!texture_.is_null()) {
        return getTextureCoords().get_width();
    } else if (!pixelBuffer_.is_null()) {
        return pixelBuffer_.get_width();
    } else {
        return 1;
    }
}

float Texture::getHeight() {
    if (!texture_.is_null()) {
        return getTextureCoords().get_height();
    } else if (!pixelBuffer_.is_null()) {
        return pixelBuffer_.get_height();
    } else {
        return 1;
    }
}

bool Texture::hasPixel(unsigned int pixelX, unsigned int pixelY) {
    if (useBitMask_) {
        return bitMask_.hasPixel(pixelX, pixelY);
    } else {
        return true;
    }
}

CL_Rectf Texture::getTextureCoords() {
    if (!texture_.is_null()) {
        return borderlessGeometry_;
    } else {
        return CL_Rectf(0, 0, 0, 0);
    }
}

CL_Rectf Texture::getNormalizedTextureCoords() {
    if (texture_.is_null()) {
        initSubTexture();
    }
    return normalizedTextureCoords_;
}

void Texture::initSubTexture() {
    CL_Size sizeWithBorder = pixelBuffer_.get_size();
    sizeWithBorder += borderWidth_*2;

    texture_ = ui::Manager::getSingleton()->provideTexture(textureUsage_, sizeWithBorder);
    borderlessGeometry_ = texture_.get_geometry();
    if (borderWidth_ > 0) {
        borderlessGeometry_.shrink(borderWidth_);
        CL_PixelBuffer bufferWithBorder = CL_PixelBufferHelp::add_border(pixelBuffer_, borderWidth_, CL_Rect(0, 0, pixelBuffer_.get_size()));

        texture_.get_texture().set_subimage(
            texture_.get_geometry().left, texture_.get_geometry().top,
            bufferWithBorder, CL_Rect(0, 0, sizeWithBorder));
    } else {
        texture_.get_texture().set_subimage(
            borderlessGeometry_.left, borderlessGeometry_.top,
            pixelBuffer_, CL_Rect(0, 0, pixelBuffer_.get_size()));
    }

    if (useBitMask_) {
        bitMask_.init(pixelBuffer_);
    }

    pixelBuffer_ = CL_PixelBuffer();

    normalizedTextureCoords_ = borderlessGeometry_;
    normalizedTextureCoords_.top /= ui::Manager::TEXTURE_GROUP_HEIGHT;
    normalizedTextureCoords_.left /= ui::Manager::TEXTURE_GROUP_WIDTH;
    normalizedTextureCoords_.right /= ui::Manager::TEXTURE_GROUP_WIDTH;
    normalizedTextureCoords_.bottom /= ui::Manager::TEXTURE_GROUP_HEIGHT;
}

CL_Texture Texture::extractSingleTexture() {
    CL_Texture ret(ui::Manager::getSingleton()->getGraphicContext(), getWidth(), getHeight());
    if (texture_.is_null()) {
        if (!pixelBuffer_.is_null()) {
            ret.set_image(pixelBuffer_);
        }

        if (useBitMask_) {
            bitMask_.init(pixelBuffer_);
        }
    } else {
        CL_PixelBuffer pxBuf = getTexture().get_pixeldata();
        // create CL_Texture from ui::Texture. very expensive, unfortunately.
        ret.set_subimage(0, 0, pxBuf, getTextureCoords());
    }
    return ret;
}

void Texture::setBorderWidth(unsigned int width) {
    borderWidth_ = width;
}

}
}
