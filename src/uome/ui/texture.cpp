
#include "texture.hpp"

#include "manager.hpp"
#include "bitmask.hpp"

namespace uome {
namespace ui {

Texture::Texture() {
}

void Texture::initPixelBuffer(unsigned int width, unsigned int height) {
    pixelBuffer_.reset(new CL_PixelBuffer(width, height, cl_rgba8));
    bitMask_.reset(new BitMask(width, height));
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
        bitMask_->init(pixelBuffer_);
        pixelBuffer_.reset();
    }

    return texture_;
}

unsigned int Texture::getWidth() {
    if (texture_) {
        return texture_->get_width();
    } else if (pixelBuffer_) {
        return pixelBuffer_->get_width();
    } else {
        return 0;
    }
}

unsigned int Texture::getHeight() {
    if (texture_) {
        return texture_->get_height();
    } else if (pixelBuffer_) {
        return pixelBuffer_->get_height();
    } else {
        return 0;
    }
}

bool Texture::hasPixel(unsigned int pixelX, unsigned int pixelY) {
    return bitMask_->hasPixel(pixelX, pixelY);
}

}
}
