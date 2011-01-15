
#include "bitmask.hpp"

namespace uome {
namespace ui {

BitMask::BitMask(unsigned int width, unsigned int height) : width_(width), height_(height) {
    bitStoreSize_ = ((width * height) / 8) + 1;
    bitStore_ = reinterpret_cast<uint8_t*>(malloc(bitStoreSize_));
    memset(bitStore_, 0, bitStoreSize_);
}

BitMask::~BitMask() {
    free(bitStore_);
}

bool BitMask::hasPixel(unsigned int pixelX, unsigned int pixelY) {
    if (pixelX >= width_ || pixelY >= height_) {
        return false;
    }

    unsigned int bitIndex = pixelY * width_ + pixelX;
    unsigned int byteIndex = bitIndex / 8;
    unsigned int bitOffset = bitIndex % 8;

    return bitStore_[byteIndex] & (1 << bitOffset);
}

void BitMask::setPixel(unsigned int pixelX, unsigned int pixelY) {
    unsigned int bitIndex = pixelY * width_ + pixelX;
    unsigned int byteIndex = bitIndex / 8;
    unsigned int bitOffset = bitIndex % 8;

    bitStore_[byteIndex] |= (1 << bitOffset);
}

void BitMask::init(boost::shared_ptr<CL_PixelBuffer> pixBuf) {
    uint32_t* bufPtr = reinterpret_cast<uint32_t*>(pixBuf->get_data());

    for (unsigned int y = 0; y < height_; ++y) {
        for (unsigned int x = 0; x < width_; ++x) {
            if (*bufPtr & 0x000000FFu) { // alpha != 0
                setPixel(x, y);
            }
            ++bufPtr;
        }
    }
}

}
}
