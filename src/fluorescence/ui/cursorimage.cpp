
#include "cursorimage.hpp"

#include "manager.hpp"
#include "texture.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>

#ifndef WIN32
#include <X11/Xcursor/Xcursor.h>
#endif

#include <boost/shared_ptr.hpp>

namespace fluo {
namespace ui {

CursorImage::CursorImage() {
#ifndef WIN32
    unixCursor_ = 0;
#endif
}

CursorImage::~CursorImage() {
#ifndef WIN32
    boost::shared_ptr<CL_DisplayWindow> clWindow = ui::Manager::getSingleton()->getMainWindow();
    if (unixCursor_) {
        XFreeCursor(clWindow->get_display(), unixCursor_);
        unixCursor_ = 0;
    }
#endif
}

void CursorImage::set(unsigned int cursorId, unsigned int artId, boost::shared_ptr<CL_DisplayWindow> window) {
    cursorId_ = cursorId;
    boost::shared_ptr<ui::Texture> texture = data::Manager::getArtLoader()->getItemTexture(artId);
    setHotspotFromTexture(texture);

#ifdef WIN32
    CL_Point hotspot(hotspotX_, hotspotY_);
    CL_SpriteDescription desc;
    desc.add_frame(*texture->getPixelBuffer().get());
    winCursor_ = CL_Cursor(*window.get(), desc, hotspot);
#else
    XcursorImage* cursorImage = XcursorImageCreate(width_, height_);
    cursorImage->xhot = hotspotX_;
    cursorImage->yhot = hotspotY_;

    // X11 expects cursor image pixels to be 32bit in argb8 format
    CL_PixelBuffer convertedBuffer = CL_PixelBuffer(width_, height_, cl_argb8);
    texture->getPixelBuffer()->convert(convertedBuffer);

    int byteCount = width_ * height_ * sizeof(XcursorPixel);
    memcpy(cursorImage->pixels, convertedBuffer.get_data(), byteCount);

    unixCursor_ = XcursorImageLoadCursor(window->get_display(), cursorImage);
#endif
}

void CursorImage::setHotspotFromTexture(boost::shared_ptr<ui::Texture> tex) {
    // TODO: find something better than busy waiting here
    while (!tex->isReadComplete()) {
        fluo::sleepMs(1);
    }

    hotspotX_ = 0;
    hotspotY_ = 0;

    width_ = tex->getWidth();
    height_ = tex->getHeight();

    uint32_t* pixel = tex->getPixelBufferData();

    // the cursors have a blue frame, with one green pixel in them marking the hotspot
    for (unsigned int x = 0; x < width_; ++x) {
        if (pixel[0 * width_ + x] != 0x0000F8FF) {
            // hot spot marker
            hotspotX_ = x;
        }

        pixel[0 * width_ + x] = 0x00000000;
        pixel[(height_-1) * width_ + x] = 0x00000000;
    }

    for (unsigned int y = 1; y < height_-1; ++y) {
        if (pixel[y * width_ + 0] != 0x0000F8FF) {
            // hot spot marker
            hotspotY_ = y;
        }

        pixel[y * width_ + 0] = 0x00000000;
        pixel[y * width_ + (width_-1)] = 0x00000000;
    }

    //LOGARG_DEBUG(LOGTYPE_UI, "Hotspot for %u found at %u/%u", cursorId_, hotspotX_, hotspotY_);
}

void CursorImage::activate() {
    boost::shared_ptr<CL_DisplayWindow> clWindow = ui::Manager::getSingleton()->getMainWindow();

#ifdef WIN32
    clWindow->set_cursor(winCursor_);
#else
    XDefineCursor(clWindow->get_display(), clWindow->get_window(), unixCursor_);
#endif

}

}
}
