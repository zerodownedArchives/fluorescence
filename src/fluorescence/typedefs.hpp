#ifndef FLUO_TYPEDEFS_HPP
#define FLUO_TYPEDEFS_HPP

#include <stdint.h>

namespace fluo {

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


struct TextType {
public:
enum {
    NORMAL = 0x00,
    BROADCAST = 0x01,
    EMOTE = 0x02,
    SYSTEM = 0x06,
    MESSAGE = 0x07,
    WHISPER = 0x08,
    YELL = 0x09,
};
};

struct Layer {
public:
enum {
    ONEHANDED = 0x01,
    TWOHANDED = 0x02,
    MOUNT = 0x19,
};
};

}


#endif
