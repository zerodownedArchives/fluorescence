#ifndef FLUO_UI_TEXTURE_HPP
#define FLUO_UI_TEXTURE_HPP

#include <ClanLib/Display/Image/pixel_buffer.h>
#include <ClanLib/Display/Image/texture_format.h>
#include <ClanLib/Display/Render/texture.h>

#include <boost/shared_ptr.hpp>

#include <data/ondemandreadable.hpp>

#include <stdint.h>

namespace fluo {
namespace ui {

class BitMask;

class Texture : public fluo::data::OnDemandReadable {
public:
    Texture();

    void initPixelBuffer(unsigned int width, unsigned int height);
    uint32_t* getPixelBufferData();

    boost::shared_ptr<CL_PixelBuffer> getPixelBuffer();

    boost::shared_ptr<CL_Texture> getTexture(bool releasePixelBuffer = true);
    void releasePixelBuffer();

    unsigned int getWidth();
    unsigned int getHeight();

    bool hasPixel(unsigned int pixelX, unsigned int pixelY);

private:
    boost::shared_ptr<CL_PixelBuffer> pixelBuffer_;
    boost::shared_ptr<CL_Texture> texture_;
    boost::shared_ptr<BitMask> bitMask_;
};
}
}

#endif
