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


#ifndef FLUO_DATA_HUESLOADER_HPP
#define FLUO_DATA_HUESLOADER_HPP

#include <boost/shared_ptr.hpp>
#include <ClanLib/Display/2D/color.h>

#include <misc/string.hpp>

#include "fullfileloader.hpp"

namespace fluo {

namespace ui {
    class Texture;
}

namespace data {

struct Hue {
    uint32_t colorTable_[32];
    uint16_t tableStart;
    uint16_t tableEnd;
    UnicodeString name_;
};

class HuesLoader {
public:
    HuesLoader(const boost::filesystem::path& path);
    ~HuesLoader();

    void readCallback(int8_t* buf,unsigned int len);

    unsigned int getHueCount() const;

    boost::shared_ptr<ui::Texture> getHuesTexture();
    uint32_t getFontRgbColor(unsigned int hue) const;
    UnicodeString getFontRgbString(unsigned int hue) const;
    CL_Colorf getFontClColor(unsigned int hue) const;

    unsigned int translateHue(unsigned int hue) const;

private:
    unsigned int hueCount_;
    Hue* hues_;

    boost::shared_ptr<ui::Texture> huesTexture_;
};

}
}

#endif
