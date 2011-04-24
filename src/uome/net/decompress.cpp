
#include "decompress.hpp"

namespace uome {
namespace net {

BinaryTree::BinaryTree() : zero_(0), one_(0), value_(0), hit_(false) {
}

BinaryTree::~BinaryTree() {
    if (zero_) {
        delete zero_;
    }

    if (one_) {
        delete one_;
    }
}

BinaryTree* BinaryTree::buildHuffmanTree() {
    BinaryTree* huffman = new BinaryTree;

    BinaryTree* cur;
    uint16_t bit = 0;
    uint16_t val = 0;

    for (unsigned int i = 0; i < 257; i++) {
        cur = huffman;
        bit = BinaryTree::Table[i][0];
        val = BinaryTree::Table[i][1];
        while (bit > 0) {
            bit--;
            if ((val & (1 << bit)) != 0) {
                if (!cur->one_)
                    cur->one_ = new BinaryTree;
                cur = cur->one_;

            } else {
                if (!cur->zero_)
                    cur->zero_ = new BinaryTree;
                cur = cur->zero_;
            }
        }

        cur->hit_ = true;
        cur->value_ = i;
    }

    return huffman;
}

uint16_t BinaryTree::Table[257][2] = {
    {0x02, 0x00}, {0x05, 0x1F}, {0x06, 0x22}, {0x07, 0x34},
    {0x07, 0x75}, {0x06, 0x28}, {0x06, 0x3B}, {0x07, 0x32},
    {0x08, 0xE0}, {0x08, 0x62}, {0x07, 0x56}, {0x08, 0x79},
    {0x09, 0x19D}, {0x08, 0x97}, {0x06, 0x2A}, {0x07, 0x57},
    {0x08, 0x71}, {0x08, 0x5B}, {0x09, 0x1CC}, {0x08, 0xA7},
    {0x07, 0x25}, {0x07, 0x4F}, {0x08, 0x66}, {0x08, 0x7D},
    {0x09, 0x191}, {0x09, 0x1CE}, {0x07, 0x3F}, {0x09, 0x90},
    {0x08, 0x59}, {0x08, 0x7B}, {0x08, 0x91}, {0x08, 0xC6},
    {0x06, 0x2D}, {0x09, 0x186}, {0x08, 0x6F}, {0x09, 0x93},
    {0x0A, 0x1CC}, {0x08, 0x5A}, {0x0A, 0x1AE}, {0x0A, 0x1C0},
    {0x09, 0x148}, {0x09, 0x14A}, {0x09, 0x82}, {0x0A, 0x19F},
    {0x09, 0x171}, {0x09, 0x120}, {0x09, 0xE7}, {0x0A, 0x1F3},
    {0x09, 0x14B}, {0x09, 0x100}, {0x09, 0x190}, {0x06, 0x13},
    {0x09, 0x161}, {0x09, 0x125}, {0x09, 0x133}, {0x09, 0x195},
    {0x09, 0x173}, {0x09, 0x1CA}, {0x09, 0x86}, {0x09, 0x1E9},
    {0x09, 0xDB}, {0x09, 0x1EC}, {0x09, 0x8B}, {0x09, 0x85},
    {0x05, 0x0A}, {0x08, 0x96}, {0x08, 0x9C}, {0x09, 0x1C3},
    {0x09, 0x19C}, {0x09, 0x8F}, {0x09, 0x18F}, {0x09, 0x91},
    {0x09, 0x87}, {0x09, 0xC6}, {0x09, 0x177}, {0x09, 0x89},
    {0x09, 0xD6}, {0x09, 0x8C}, {0x09, 0x1EE}, {0x09, 0x1EB},
    {0x09, 0x84}, {0x09, 0x164}, {0x09, 0x175}, {0x09, 0x1CD},
    {0x08, 0x5E}, {0x09, 0x88}, {0x09, 0x12B}, {0x09, 0x172},
    {0x09, 0x10A}, {0x09, 0x8D}, {0x09, 0x13A}, {0x09, 0x11C},
    {0x0A, 0x1E1}, {0x0A, 0x1E0}, {0x09, 0x187}, {0x0A, 0x1DC},
    {0x0A, 0x1DF}, {0x07, 0x74}, {0x09, 0x19F}, {0x08, 0x8D},
    {0x08, 0xE4}, {0x07, 0x79}, {0x09, 0xEA}, {0x09, 0xE1},
    {0x08, 0x40}, {0x07, 0x41}, {0x09, 0x10B}, {0x09, 0xB0},
    {0x08, 0x6A}, {0x08, 0xC1}, {0x07, 0x71}, {0x07, 0x78},
    {0x08, 0xB1}, {0x09, 0x14C}, {0x07, 0x43}, {0x08, 0x76},
    {0x07, 0x66}, {0x07, 0x4D}, {0x09, 0x8A}, {0x06, 0x2F},
    {0x08, 0xC9}, {0x09, 0xCE}, {0x09, 0x149}, {0x09, 0x160},
    {0x0A, 0x1BA}, {0x0A, 0x19E}, {0x0A, 0x39F}, {0x09, 0xE5},
    {0x09, 0x194}, {0x09, 0x184}, {0x09, 0x126}, {0x07, 0x30},
    {0x08, 0x6C}, {0x09, 0x121}, {0x09, 0x1E8}, {0x0A, 0x1C1},
    {0x0A, 0x11D}, {0x0A, 0x163}, {0x0A, 0x385}, {0x0A, 0x3DB},
    {0x0A, 0x17D}, {0x0A, 0x106}, {0x0A, 0x397}, {0x0A, 0x24E},
    {0x07, 0x2E}, {0x08, 0x98}, {0x0A, 0x33C}, {0x0A, 0x32E},
    {0x0A, 0x1E9}, {0x09, 0xBF}, {0x0A, 0x3DF}, {0x0A, 0x1DD},
    {0x0A, 0x32D}, {0x0A, 0x2ED}, {0x0A, 0x30B}, {0x0A, 0x107},
    {0x0A, 0x2E8}, {0x0A, 0x3DE}, {0x0A, 0x125}, {0x0A, 0x1E8},
    {0x09, 0xE9}, {0x0A, 0x1CD}, {0x0A, 0x1B5}, {0x09, 0x165},
    {0x0A, 0x232}, {0x0A, 0x2E1}, {0x0B, 0x3AE}, {0x0B, 0x3C6},
    {0x0B, 0x3E2}, {0x0A, 0x205}, {0x0A, 0x29A}, {0x0A, 0x248},
    {0x0A, 0x2CD}, {0x0A, 0x23B}, {0x0B, 0x3C5}, {0x0A, 0x251},
    {0x0A, 0x2E9}, {0x0A, 0x252}, {0x09, 0x1EA}, {0x0B, 0x3A0},
    {0x0B, 0x391}, {0x0A, 0x23C}, {0x0B, 0x392}, {0x0B, 0x3D5},
    {0x0A, 0x233}, {0x0A, 0x2CC}, {0x0B, 0x390}, {0x0A, 0x1BB},
    {0x0B, 0x3A1}, {0x0B, 0x3C4}, {0x0A, 0x211}, {0x0A, 0x203},
    {0x09, 0x12A}, {0x0A, 0x231}, {0x0B, 0x3E0}, {0x0A, 0x29B},
    {0x0B, 0x3D7}, {0x0A, 0x202}, {0x0B, 0x3AD}, {0x0A, 0x213},
    {0x0A, 0x253}, {0x0A, 0x32C}, {0x0A, 0x23D}, {0x0A, 0x23F},
    {0x0A, 0x32F}, {0x0A, 0x11C}, {0x0A, 0x384}, {0x0A, 0x31C},
    {0x0A, 0x17C}, {0x0A, 0x30A}, {0x0A, 0x2E0}, {0x0A, 0x276},
    {0x0A, 0x250}, {0x0B, 0x3E3}, {0x0A, 0x396}, {0x0A, 0x18F},
    {0x0A, 0x204}, {0x0A, 0x206}, {0x0A, 0x230}, {0x0A, 0x265},
    {0x0A, 0x212}, {0x0A, 0x23E}, {0x0B, 0x3AC}, {0x0B, 0x393},
    {0x0B, 0x3E1}, {0x0A, 0x1DE}, {0x0B, 0x3D6}, {0x0A, 0x31D},
    {0x0B, 0x3E5}, {0x0B, 0x3E4}, {0x0A, 0x207}, {0x0B, 0x3C7},
    {0x0A, 0x277}, {0x0B, 0x3D4}, {0x08, 0xC0}, {0x0A, 0x162},
    {0x0A, 0x3DA}, {0x0A, 0x124}, {0x0A, 0x1B4}, {0x0A, 0x264},
    {0x0A, 0x33D}, {0x0A, 0x1D1}, {0x0A, 0x1AF}, {0x0A, 0x39E},
    {0x0A, 0x24F}, {0x0B, 0x373}, {0x0A, 0x249}, {0x0B, 0x372},
    {0x09, 0x167}, {0x0A, 0x210}, {0x0A, 0x23A}, {0x0A, 0x1B8},
    {0x0B, 0x3AF}, {0x0A, 0x18E}, {0x0A, 0x2EC}, {0x07, 0x62},
    {0x04, 0x0D}
};


Decompress::Decompress() {
    huffmanTree_ = BinaryTree::buildHuffmanTree();
}

Decompress::~Decompress() {
    delete huffmanTree_;
}

unsigned int Decompress::huffmanDecompress(int8_t* dst, unsigned int dstLen, int8_t* src, unsigned int srcLen) {
    unsigned int readBits = 0;
    unsigned int srcIndex = 0;
    unsigned int dstIndex = 0;

    uint8_t* udst = reinterpret_cast<uint8_t*>(dst);
    uint8_t* usrc = reinterpret_cast<uint8_t*>(src);

    uint8_t readValue;

    BinaryTree* curTree = huffmanTree_;

    while (srcIndex < srcLen && dstIndex < dstLen) {
        if (readBits <= 0) {
            readValue = usrc[srcIndex];
            srcIndex++;
            readBits = 8;
        }

        do {
            readBits--;
            if ((readValue & (1 << readBits)) != 0) {
                curTree = curTree->one_;
            } else {
                curTree = curTree->zero_;
            }

            if (curTree->hit_) {
                if (curTree->value_ == 256) {
                    readBits = 0;
                    curTree = huffmanTree_;
                } else {
                    udst[dstIndex++]= curTree->value_;
                }
                curTree = huffmanTree_;
            }
        } while (readBits > 0);
    }

    return dstIndex;
}


}
}
