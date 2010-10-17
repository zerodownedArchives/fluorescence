#ifndef UOME_DATA_MAPBLOCK_HPP
#define UOME_DATA_MAPBLOCK_HPP

#include <boost/filesystem/fstream.hpp>

namespace uome {
namespace data {

struct RawMapTile {
    uint16_t tileId_;
    int8_t z_;

    void read(int8_t* buf, unsigned int len) {
        tileId_ = *(reinterpret_cast<uint16_t*>(buf));
        z_ = *(buf + 2);
    }
};

class RawMapBlock {
public:
    ~RawMapBlock() {
        printf("map block dtor\n");
    }
    int32_t header_;
    RawMapTile tiles_[64];

    void read(int8_t* buf, unsigned int len) {
        int8_t* ptr = buf;
        header_ = *(reinterpret_cast<int32_t*>(ptr));
        ptr += 4;

        for (int i = 0; i < 64; i++) {
            tiles_[i].read(ptr, 3);
            ptr += 3;
        }
    }
};

}
}

#endif
