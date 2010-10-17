#ifndef UOME_DATA_INDEXBLOCK_HPP
#define UOME_DATA_INDEXBLOCK_HPP

#include <iostream>

namespace uome {
namespace data {

struct IndexBlock {
    int32_t offset_;
    int32_t length_;
    int32_t extra_;

    void read(int8_t* buf, uint32_t len) {
        offset_ = *(reinterpret_cast<int32_t*>(buf));
        length_ = *(reinterpret_cast<int32_t*>(buf + 4));
        extra_ = *(reinterpret_cast<int32_t*>(buf + 8));
    }
};

}
}

#endif
