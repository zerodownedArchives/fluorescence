#ifndef UOME_DATA_TILEDATA_HPP
#define UOME_DATA_TILEDATA_HPP

#include <stdint.h>
#include <string.hpp>

namespace uome {
namespace data {

struct LandTileRecord {
    uint32_t flags_;
    uint16_t textureId_;
    String name_;

    void read(int8_t* buf, unsigned int len);
};

struct StaticTileRecord {
    uint32_t flags_;
    uint8_t weight_;
    uint8_t quality_;
    uint16_t unknown1_;
    uint8_t unknown2_;
    uint8_t quantity_;
    uint16_t animId_;
    uint8_t unknown3_;
    uint8_t hue_;
    uint8_t unknown4_;
    uint8_t unknown5_;
    uint8_t height_;
    String name_;

    void read(int8_t* buf, unsigned int len);
};

struct LandTileBlock {
    int32_t header_;
    LandTileRecord records_[32];

    void read(int8_t* buf, unsigned int len);
};

struct StaticTileBlock {
    int32_t header_;
    StaticTileRecord records_[32];

    void read(int8_t* buf, unsigned int len);
};

}
}

#endif
