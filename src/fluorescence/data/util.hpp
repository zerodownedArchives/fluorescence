#ifndef FLUO_DATA_UTIL_HPP
#define FLUO_DATA_UTIL_HPP

namespace fluo {
namespace data {

class Util {
public:
    static uint8_t getColorR(uint16_t color) {
        return ( ( color >> 10 ) & 0x1F ) << 3;
    }

    static uint8_t getColorG(uint16_t color) {
        return ( ( color >> 5 ) & 0x1F ) << 3;
    }

    static uint8_t getColorB(uint16_t color) {
        return ( color & 0x1F ) << 3;
    }

    static uint32_t getColorRGBA(uint16_t color) {
        return ( getColorR(color)<<24 ) |
               ( getColorG( color ) << 16 ) |
               ( getColorB( color ) << 8 ) |
               ((color) ? 0xFF : 0x00);
    }

};

}
}
#endif
