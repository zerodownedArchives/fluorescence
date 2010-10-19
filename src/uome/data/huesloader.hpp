#ifndef UOME_DATA_HUESLOADER_HPP
#define UOME_DATA_HUESLOADER_HPP

#include <string.hpp>

#include "fullfileloader.hpp"

namespace uome {
namespace data {

struct Hue {
    uint32_t colorTable_[32];
    uint16_t tableStart;
    uint16_t tableEnd;
    String name_;
};

class HuesLoader {
public:
    HuesLoader(const boost::filesystem::path& path);
    ~HuesLoader();

    void readCallback(int8_t* buf,unsigned int len);

    unsigned int getHueCount() const;

    const uint32_t* getColorTable(unsigned int id) const;

private:
    unsigned int hueCount_;
    Hue* hues_;
};

}
}

#endif
