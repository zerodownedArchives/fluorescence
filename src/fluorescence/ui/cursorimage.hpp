#ifndef FLUO_UI_CURSORIMAGE_HPP
#define FLUO_UI_CURSORIMAGE_HPP

#include <boost/shared_ptr.hpp>

#ifdef WIN32
#include <ClanLib/Display/Window/cursor.h>
#else
#include <X11/Xlib.h>
#endif

class CL_DisplayWindow;

namespace fluo {
namespace ui {

class Texture;

class CursorImage {
public:
    CursorImage();
    ~CursorImage();

    void set(unsigned int cursorId, unsigned int artId, boost::shared_ptr<CL_DisplayWindow> window);

    void activate();

private:
    unsigned int cursorId_;

    int hotspotX_;
    int hotspotY_;

    unsigned int width_;
    unsigned int height_;

    void setHotspotFromTexture(boost::shared_ptr<ui::Texture> texture);

// on windows, we can just use the cursor thingy provided by clanlib.
// on unix, this does not work (yet), so we have to set the cursor directly
// windows stuff untested - not even compiled yet
#ifdef WIN32
    CL_Cursor winCursor_;
#else
    Cursor unixCursor_;
#endif

};

}
}

#endif
