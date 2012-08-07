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
    UnicodeCharacter();
    UnicodeCharacter(unsigned int charCode, int xOffset, int yOffset, int width, int height);

    ~UnicodeCharacter();

    unsigned int charCode_;
    int xOffset_;
    int yOffset_;
    unsigned int width_;
    unsigned int height_;
    uint8_t* data_;

    unsigned int getTotalWidth() const;
    unsigned int getTotalHeight() const;

    void debugPrintToConsole() const;
};

class UniFontLoader {
private:
    typedef std::map<unsigned int, boost::shared_ptr<UnicodeCharacter> > MapType;

public:
    UniFontLoader(const boost::filesystem::path& mulPath);

    boost::shared_ptr<UnicodeCharacter> getCharacter(unsigned int character);

    unsigned int getMaxHeight();

private:
    boost::filesystem::ifstream stream_;

    MapType cache_;

    unsigned int offsets_[0x10000];

    unsigned int maxHeight_;
};

}
}

#endif
