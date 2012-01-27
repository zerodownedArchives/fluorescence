/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


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
