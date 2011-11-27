#ifndef FLUO_DATA_UTIL_HPP
#define FLUO_DATA_UTIL_HPP

#include <stdint.h>

namespace fluo {
namespace data {

class Util {
public:
    static uint8_t getColorR(uint16_t color);
    static uint8_t getColorG(uint16_t color);
    static uint8_t getColorB(uint16_t color);
    static uint32_t getColorRGBA(uint16_t color);
};

}
}

#endif
