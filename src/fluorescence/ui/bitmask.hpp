#ifndef FLUO_UI_BITMASK_HPP
#define FLUO_UI_BITMASK_HPP

#include <boost/shared_ptr.hpp>
#include <ClanLib/Display/Image/pixel_buffer.h>

#include <stdint.h>

namespace fluo {
namespace ui {

/// This class is used to store pixel visibility for a texture
class BitMask {
public:
    BitMask();
    ~BitMask();

    void init(boost::shared_ptr<CL_PixelBuffer> pixBuf);

    bool hasPixel(unsigned int pixelX, unsigned int pixelY);

private:
    unsigned int width_;
    unsigned int height_;

    unsigned int bitStoreSize_;
    uint8_t* bitStore_;

    void setPixel(unsigned int pixelX, unsigned int pixelY);
};

}
}

#endif
