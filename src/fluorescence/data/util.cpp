
#include "util.hpp"

namespace fluo {
namespace data {

uint8_t Util::getColorR(uint16_t color) {
    return ( ( color >> 10 ) & 0x1F ) << 3;
}

uint8_t Util::getColorG(uint16_t color) {
    return ( ( color >> 5 ) & 0x1F ) << 3;
}

uint8_t Util::getColorB(uint16_t color) {
    return ( color & 0x1F ) << 3;
}

uint32_t Util::getColorRGBA(uint16_t color) {
    return ( getColorR(color)<<24 ) |
           ( getColorG( color ) << 16 ) |
           ( getColorB( color ) << 8 ) |
           ((color) ? 0xFF : 0x00);
}

}
}
