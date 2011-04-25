#ifndef UOME_UI_CURSORMANAGER_HPP
#define UOME_UI_CURSORMANAGER_HPP

#include <boost/shared_ptr.hpp>

#include <misc/config.hpp>

#include "cursorimage.hpp"

class CL_DisplayWindow;

namespace uome {
namespace ui {

struct CursorType {
    enum {
        GAME_NORTHWEST = 0,
        GAME_NORTH = 1,
        GAME_NORTHEAST = 2,
        GAME_EAST = 3,
        GAME_SOUTHEAST = 4,
        GAME_SOUTH = 5,
        GAME_SOUTHWEST = 6,
        GAME_WEST = 7,
        FIST = 8,
        SELECTION = 9,
        PICK_UP = 10,
        BONE_HAND = 11,
        TARGET = 12,
        LOADING = 13,
        FEATHER = 14,
        NEEDLE = 15,

        COUNT,
    };
};

class CursorManager {
public:
    CursorManager(Config& config, boost::shared_ptr<CL_DisplayWindow> window);

    void setWarMode(bool value);
    void setCursor(unsigned int id);

private:
    bool warMode_;
    unsigned int warmodeArtOffset_;
    unsigned int currentCursorId_;

    CursorImage cursorImages_[CursorType::COUNT * 2];

    void setCursorImage(unsigned int id, unsigned int artId, bool warMode, boost::shared_ptr<CL_DisplayWindow> window);

    void updateCursor();
};

}
}

#endif
