
#include "texture.hpp"

namespace uome {
namespace ui {

Texture::Texture() {
}

void Texture::initPixelBuffer(unsigned int width, unsigned int height, CL_TextureFormat format) {
    pixelBuffer_.reset(new CL_PixelBuffer(width, height, format));
}

void* Texture::getPixelBufferData() {
    return pixelBuffer_->get_data();
}

CL_PixelBuffer* Texture::getPixelBuffer() {
    return pixelBuffer_.get();
}

CL_Texture* Texture::getTexture() {
    if (!texture_.get()) {
        texture_ .reset(ui::Manager::getSingleton()->provideTexture(pixelBuffer_->get_width(), pixelBuffer_->get_height()));
        texture_->set_image(*(pixelBuffer_));
    }

    return texture_.get();
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

}
}
