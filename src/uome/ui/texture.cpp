
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

boost::shared_ptr<CL_Texture> Texture::getTexture() {
    if (!texture_.get()) {
        texture_ = boost::shared_ptr<CL_Texture>(ui::Manager::getSingleton()->provideTexture(pixelBuffer_->get_width(), pixelBuffer_->get_height()));
        texture_->set_image(*(pixelBuffer_));
    }

    return texture_;
}

}
}
