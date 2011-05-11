#ifndef UOME_TYPEDEFS_HPP
#define UOME_TYPEDEFS_HPP


namespace uome {

struct Direction {
public:
enum {
    NE = 0x00,
    E = 0x01,
    SE = 0x02,
    S = 0x03,
    SW = 0x04,
    W = 0x05,
    NW = 0x06,
    N = 0x07,

    RUNNING = 0x80,
};
};

}


#endif
