#ifndef FLUO_NET_DECOMPRESS_HPP
#define FLUO_NET_DECOMPRESS_HPP

#include <stdint.h>

namespace fluo {
namespace net {

class BinaryTree {
friend class Decompress;

public:
    static BinaryTree* buildHuffmanTree();

private:
    BinaryTree();
    ~BinaryTree();

    BinaryTree* zero_;
    BinaryTree* one_;
    uint16_t value_;
    bool hit_;

    static uint16_t Table[257][2];
};

class Decompress {
public:
    Decompress();
    ~Decompress();

    unsigned int huffmanDecompress(int8_t* dst, unsigned int dstLen, int8_t* src, unsigned int srcLen);

private:
    BinaryTree* huffmanTree_;
};

}
}

#endif
