#ifndef FLUO_DATA_UNIFONTLOADER_HPP
#define FLUO_DATA_UNIFONTLOADER_HPP

#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <misc/string.hpp>

namespace fluo {

namespace ui {
    class Texture;
}

namespace data {

class UnicodeCharacter {
public:
    enum {
        TRANSPARENT = 0,
        BORDER = 1,
        LETTER = 2,
    };

    UnicodeCharacter();
    UnicodeCharacter(unsigned int xOffset, unsigned int yOffset, unsigned int width, unsigned int height);

    unsigned int xOffset_;
    unsigned int yOffset_;
    unsigned int width_;
    unsigned int height_;
    uint8_t* data_;

    unsigned int getTotalWidth() const;
    unsigned int getTotalHeight() const;
};

class UniFontLoader {
private:
    typedef std::map<unsigned int, boost::shared_ptr<UnicodeCharacter> > MapType;

public:
    UniFontLoader(const boost::filesystem::path& mulPath);

    boost::shared_ptr<ui::Texture> getText(const UnicodeString& text, bool border, unsigned int maxWidth, uint32_t color);

private:
    boost::filesystem::ifstream stream_;

    MapType cache_;

    unsigned int offsets_[0x10000];

    unsigned int maxHeight_;

    boost::shared_ptr<UnicodeCharacter> getCharacter(unsigned int character);
};

}
}

#endif
