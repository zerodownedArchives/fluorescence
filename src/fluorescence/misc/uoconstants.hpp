#ifndef FLUO_MISC_UOCONSTANTS_HPP
#define FLUO_MISC_UOCONSTANTS_HPP

namespace fluo {
namespace misc {

struct Direction {
    enum {
        NORTH = 0x00,
        NORTHEAST = 0x01,
        EAST = 0x02,
        SOUTHEAST = 0x03,
        SOUTH = 0x04,
        SOUTHWEST = 0x05,
        WEST = 0x06,
        NORTHWEST = 0x07,
    }
};
};
}


#endif
