
#include "texture.hpp"

namespace uome {
namespace ui {

Texture::Texture() : readComplete_(false) {
}

void Texture::initPixelBuffer(unsigned int width, unsigned int height, CL_TextureFormat format) {
    pixelBuffer_.reset(new CL_PixelBuffer(width, height, format));
}

void* Texture::getPixelBufferData() {
    return pixelBuffer_->get_data();
}

}
}
