
#include "tiledata.hpp"

namespace uome {
namespace data {

void LandTileRecord::read(int8_t* buf, unsigned int len) {
    flags_ = *(reinterpret_cast<uint32_t*>(buf));
    flags_ = *(reinterpret_cast<uint16_t*>(buf + 4));
    name_ = StringConverter::fromUtf8(reinterpret_cast<char*>(buf + 6), 20);
}

void StaticTileRecord::read(int8_t* buf, unsigned int len) {
    int8_t* ptr = buf;
    flags_ = *(reinterpret_cast<uint32_t*>(ptr));
    ptr += 4;

    weight_ = *(reinterpret_cast<uint8_t*>(ptr));
    ++ptr;

    quality_ = *(reinterpret_cast<uint8_t*>(ptr));
    ++ptr;

    unknown1_ = *(reinterpret_cast<uint16_t*>(ptr));
    ptr += 2;

    unknown2_ = *(reinterpret_cast<uint8_t*>(ptr));
    ++ptr;

    quantity_ = *(reinterpret_cast<uint8_t*>(ptr));
    ++ptr;

    animId_ = *(reinterpret_cast<uint16_t*>(ptr));
    ptr += 2;

    unknown3_ = *(reinterpret_cast<uint8_t*>(ptr));
    ++ptr;

    hue_ = *(reinterpret_cast<uint8_t*>(ptr));
    ++ptr;

    unknown4_ = *(reinterpret_cast<uint8_t*>(ptr));
    ++ptr;

    unknown5_ = *(reinterpret_cast<uint8_t*>(ptr));
    ++ptr;

    height_ = *(reinterpret_cast<uint8_t*>(ptr));
    ++ptr;

    name_ = StringConverter::fromUtf8(reinterpret_cast<char*>(ptr), 20);
}

void LandTileBlock::read(int8_t* buf, unsigned int len) {
    int8_t* ptr = buf;
    header_ = *(reinterpret_cast<uint32_t*>(ptr));
    ptr += 4;

    for (unsigned int i = 0; i < 32; i++) {
        records_[i].read(ptr, 26);
        ptr += 26;
    }
}

void StaticTileBlock::read(int8_t* buf, unsigned int len) {
    int8_t* ptr = buf;
    header_ = *(reinterpret_cast<uint32_t*>(ptr));
    ptr += 4;

    for (unsigned int i = 0; i < 32; i++) {
        records_[i].read(ptr, 37);
        ptr += 37;
    }
}

}
}
