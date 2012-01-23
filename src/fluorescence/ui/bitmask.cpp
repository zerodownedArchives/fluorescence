
#include "bitmask.hpp"

#include <string.h>

namespace fluo {
namespace ui {

BitMask::BitMask() : width_(0), height_(0), bitStoreSize_(0), bitStore_(NULL) {

}

BitMask::~BitMask() {
    if (bitStore_) {
        free(bitStore_);
        bitStore_ = NULL;
    }
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
    init(*pixBuf.get());
}

void BitMask::init(const CL_PixelBuffer& pixBuf) {
    width_ = pixBuf.get_width();
    height_ = pixBuf.get_height();
    bitStoreSize_ = ((width_ * height_) / 8) + 1;
    bitStore_ = reinterpret_cast<uint8_t*>(malloc(bitStoreSize_));
    memset(bitStore_, 0, bitStoreSize_);

    const uint32_t* bufPtr = reinterpret_cast<const uint32_t*>(pixBuf.get_data());

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
