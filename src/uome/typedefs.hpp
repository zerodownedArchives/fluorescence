#ifndef UOME_TYPEDEFS_HPP
#define UOME_TYPEDEFS_HPP

#include <stdint.h>

namespace uome {

typedef uint32_t Serial;

struct Direction {
public:
enum {
    N = 0x00,
    NE = 0x01,
    E = 0x02,
    SE = 0x03,
    S = 0x04,
    SW = 0x05,
    W = 0x06,
    NW = 0x07,

    RUNNING = 0x80,
};
};

}


#endif
