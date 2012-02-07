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

namespace fluo {
namespace ui {

Texture::Texture(bool useBitMask) : useBitMask_(useBitMask) {
}

void Texture::initPixelBuffer(unsigned int width, unsigned int height) {
    pixelBuffer_.reset(new CL_PixelBuffer(width, height, cl_rgba8));

    memset(getPixelBufferData(), 0, width * height * sizeof(uint32_t));
}

uint32_t* Texture::getPixelBufferData() {
    return reinterpret_cast<uint32_t*>(pixelBuffer_->get_data());
}

boost::shared_ptr<CL_PixelBuffer> Texture::getPixelBuffer() {
    return pixelBuffer_;
}

boost::shared_ptr<CL_Texture> Texture::getTexture() {
    if (!texture_.get()) {
        texture_.reset(ui::Manager::getSingleton()->provideTexture(pixelBuffer_->get_width(), pixelBuffer_->get_height()));
        texture_->set_image(*(pixelBuffer_));

        if (useBitMask_) {
            bitMask_.init(pixelBuffer_);
        }

        pixelBuffer_.reset();
    }

    return texture_;
}

void Texture::setTexture(CL_PixelBuffer& pixBuf) {
    if (!texture_.get()) {
        pixelBuffer_.reset(new CL_PixelBuffer(pixBuf.get_width(), pixBuf.get_height(), cl_rgba8));
        memcpy(pixelBuffer_->get_data(), pixBuf.get_data(), pixBuf.get_width() * pixBuf.get_height() * 4);
        
        if (useBitMask_) {
            bitMask_.init(pixelBuffer_);
        }
        
        setReadComplete();
    }
}

float Texture::getWidth() {
    if (texture_) {
        return texture_->get_width();
    } else if (pixelBuffer_) {
        return pixelBuffer_->get_width();
    } else {
        return 1;
    }
}

float Texture::getHeight() {
    if (texture_) {
        return texture_->get_height();
    } else if (pixelBuffer_) {
        return pixelBuffer_->get_height();
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

}
}
