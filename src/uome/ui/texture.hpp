#ifndef UOME_UI_TEXTURE_HPP
#define UOME_UI_TEXTURE_HPP

#include <ClanLib/Display/Image/pixel_buffer.h>
#include <ClanLib/Display/Image/texture_format.h>

#include <boost/shared_ptr.hpp>

#include <data/ondemandreadable.hpp>

namespace uome {
namespace ui {

class Texture : public uome::data::OnDemandReadable {
public:
    Texture();

    void initPixelBuffer(unsigned int width, unsigned int height, CL_TextureFormat format);
    void* getPixelBufferData();

    boost::shared_ptr<CL_PixelBuffer> getPixelBuffer() { return pixelBuffer_; }

private:
    boost::shared_ptr<CL_PixelBuffer> pixelBuffer_;
};
}
}

#endif
