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



#include "unifontloader.hpp"

#include <list>
#include <string.h>
#include <boost/filesystem/operations.hpp>
#include <unicode/schriter.h>

#include <misc/exception.hpp>
#include <misc/log.hpp>

#include <ui/texture.hpp>


#define BORDER_WIDTH 1

namespace fluo {
namespace data {

UnicodeCharacter::UnicodeCharacter() :
        charCode_(0), xOffset_(0), yOffset_(0), width_(0), height_(0), data_(NULL) {
}

UnicodeCharacter::UnicodeCharacter(unsigned int charCode, int xOffset, int yOffset, unsigned int width, unsigned int height) :
        charCode_(charCode), xOffset_(xOffset), yOffset_(yOffset), width_(width), height_(height) {

    data_ = reinterpret_cast<uint8_t*>(malloc(width_ * height_));

    memset(data_, 0, width_ * height_);
}

UnicodeCharacter::~UnicodeCharacter() {
    if (data_) {
        free(data_);
        data_ = NULL;
    }
}

unsigned int UnicodeCharacter::getTotalWidth() const {
    return xOffset_ + width_;
}

unsigned int UnicodeCharacter::getTotalHeight() const {
    return yOffset_ + height_;
}

void UnicodeCharacter::debugPrintToConsole() const {
    LOG_DEBUG << "UnicodeCharacter code=" << charCode_ << " xOff=" << xOffset_ << " yOff=" << yOffset_ << " width=" << width_ << " height=" << height_ << std::endl;
    for (unsigned int y = 0; y < height_; ++y) {
        for (unsigned int x = 0; x < width_; ++x) {
            if (data_[y * width_ + x] == 1) {
                printf("o");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    LOG_DEBUG << "end UnicodeCharacter" << std::endl;
}


UniFontLoader::UniFontLoader(const boost::filesystem::path& path) : maxHeight_(0) {
    if (!boost::filesystem::exists(path) || !boost::filesystem::is_regular_file(path)) {
        throw Exception("File not found");
    }

    stream_.open(path, std::ios_base::binary);
    if (!stream_.is_open()) {
        throw Exception("Error opening stream");
    }

    // read offsets
    stream_.read(reinterpret_cast<char*>(offsets_), 0x40000);


    // store dummy blank character
    boost::shared_ptr<UnicodeCharacter> space(new UnicodeCharacter(' ', 0, 0, 5, 1));
    cache_[' '] = space;

    // load a few characters to get an estimated font height
    getCharacter('M');
    getCharacter('W');

    getCharacter(' ');
}

boost::shared_ptr<UnicodeCharacter> UniFontLoader::getCharacter(unsigned int character) {
    UniFontLoader::MapType::iterator iter = cache_.find(character);
    if (iter != cache_.end()) {
        boost::shared_ptr<UnicodeCharacter> ret = iter->second;
        return ret;
    }

    if (character >= 0x40000 || offsets_[character] == 0) {
        boost::shared_ptr<UnicodeCharacter> ret;
        return ret;
    }

    stream_.seekg(offsets_[character], std::ios_base::beg);
    int8_t charHeader[4];
    stream_.read(reinterpret_cast<char*>(charHeader), 4);

    boost::shared_ptr<UnicodeCharacter> ret(new UnicodeCharacter(character, charHeader[0], charHeader[1], charHeader[2], charHeader[3]));

    if (ret->getTotalHeight() > maxHeight_) {
        maxHeight_ = ret->getTotalHeight();
    }

    static uint8_t scanline[1024];
    static uint8_t pow2[] = { 1, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7 };

    unsigned int lineWidth = (ret->width_ - 1) / 8 + 1;
    stream_.read(reinterpret_cast<char*>(scanline), lineWidth * ret->height_);
    uint8_t* scanlinePtr = scanline - 1; // because it is immediately increased again

    for (unsigned int y = 0; y < ret->height_; ++y) {
        unsigned int bitIndex = 7;
        ++scanlinePtr;

        for (unsigned int x = 0; x < ret->width_; ++x) {
            if ((*scanlinePtr & pow2[bitIndex]) != 0) {
                ret->data_[y * ret->width_ + x] = 1;
            }

            if (bitIndex == 0) {
                bitIndex = 7;

                if (x < ret->width_-1) { // only advance pointer if this is not already the last byte for this char
                    ++scanlinePtr;
                }
            } else {
                bitIndex--;
            }
        }
    }

    cache_[character] = ret;

    return ret;
}

unsigned int UniFontLoader::getMaxHeight() {
    return maxHeight_;
}

#undef BORDER_WIDTH


}
}
