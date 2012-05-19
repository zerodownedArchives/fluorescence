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



#include "huesloader.hpp"

#include <sstream>
#include <boost/bind.hpp>
#include <ClanLib/Display/Image/pixel_buffer.h>

#include "util.hpp"

#include <ui/manager.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace data {

HuesLoader::HuesLoader(const boost::filesystem::path& path) {
    FullFileLoader loader(path);
    loader.read(boost::bind(&HuesLoader::readCallback, this, _1, _2));
}

HuesLoader::~HuesLoader() {
    if (hues_) {
        delete [] hues_;
        hues_ = NULL;
    }
}

void HuesLoader::readCallback(int8_t* buf, unsigned int len) {
    hueCount_ = (len / ((88 * 8) + 4)) * 8;
    hues_ = new Hue[hueCount_];

    uint16_t* ptr = reinterpret_cast<uint16_t*>(buf);

    for (unsigned int i = 0; i < hueCount_; ++i) {
        // jump header
        if ((i % 8) == 0) {
            ptr += 2;
        }

        for (unsigned int j = 0; j < 32; ++j) {
            hues_[i].colorTable_[j] = Util::getColorRGBA(*ptr);
            ++ptr;
        }

        hues_[i].tableStart = *ptr;
        ++ptr;
        hues_[i].tableEnd = *ptr;
        ++ptr;

        hues_[i].name_ = StringConverter::fromUtf8(reinterpret_cast<const char*>(ptr), 20);
        ptr += 10;
    }

    LOG_DEBUG << "Total read bytes: " << (reinterpret_cast<int8_t*>(ptr) - buf) << " len: " << len << std::endl;
}

unsigned int HuesLoader::getHueCount() const {
    return hueCount_;
}

CL_Texture& HuesLoader::getHuesTexture() {
    if (huesTexture_.is_null()) {
        huesTexture_ = ui::Manager::getSingleton()->providerRenderBufferTexture(CL_Size(32, hueCount_ + 1));
        CL_PixelBuffer pixelBuffer(32, hueCount_ + 1, cl_rgba8);

        uint32_t* pxBuf = reinterpret_cast<uint32_t*>(pixelBuffer.get_data());

        // set hue 0 to black
        for (unsigned int pxIdx = 0; pxIdx < 32; ++pxIdx) {
            *pxBuf++ = 0;
        }

        for (unsigned int hueIdx = 0; hueIdx < hueCount_; ++hueIdx) {
            for (unsigned int pxIdx = 0; pxIdx < 32; ++pxIdx) {
                *pxBuf++ = hues_[hueIdx].colorTable_[pxIdx];
            }
        }

        huesTexture_.set_image(pixelBuffer);
    }

    return huesTexture_;
}

uint32_t HuesLoader::getFontRgbColor(unsigned int hue) const {
    if (hue >= hueCount_) {
        LOG_WARN << "Trying to access too high hue index " << hue << std::endl;
        hue = 0;
    } else if (hue >= 1) {
        hue--;
    }

    return hues_[hue].colorTable_[31];
}

unsigned int HuesLoader::translateHue(unsigned int hue) const {
    unsigned int ret = hue;

    // skin colors use strange magic values
    if (hue >= 33770) {
        ret -= 32768;
    }

    return ret;
}

UnicodeString HuesLoader::getFontRgbString(unsigned int hue) const {
    uint32_t rgb = getFontRgbColor(hue);
        
    std::stringstream sstr;
    sstr << "#" << std::hex << ((rgb >> 24) & 0xFF) << ((rgb >> 16) & 0xFF) << ((rgb >> 8) & 0xFF);
    
    return StringConverter::fromUtf8(sstr.str());
}

CL_Colorf HuesLoader::getFontClColor(unsigned int hue) const {
    uint32_t rgb = getFontRgbColor(hue);
    
    int r = (rgb >> 24) & 0xFF;
    int g = (rgb >> 16) & 0xFF;
    int b = (rgb >> 8) & 0xFF;
    
    return CL_Colorf(r, g, b);
}

}
}
