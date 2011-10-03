
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
        xOffset_(0), yOffset_(0), width_(0), height_(0), data_(NULL) {
}

UnicodeCharacter::UnicodeCharacter(unsigned int xOffset, unsigned int yOffset, unsigned int width, unsigned int height, bool border) :
        xOffset_(xOffset), yOffset_(yOffset), width_(width), height_(height) {

    if (border) {
        width_ += BORDER_WIDTH * 2;
        height_ += BORDER_WIDTH * 2;
    }

    data_ = reinterpret_cast<uint8_t*>(malloc(width_ * height_));
    memset(data_, UnicodeCharacter::TRANSPARENT, width_ * height_);
}

unsigned int UnicodeCharacter::getTotalWidth() const {
    return xOffset_ + width_;
}

unsigned int UnicodeCharacter::getTotalHeight() const {
    return yOffset_ + height_;
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

    // load a few characters to get an estimated font height
    //getCharacter('M', true);
    //getCharacter('W', true);
    getCharacter('M', false);
    getCharacter('W', false);
}

boost::shared_ptr<UnicodeCharacter> UniFontLoader::getCharacter(unsigned int character, bool border) {
    if (character >= 0x40000 || offsets_[character] == 0) {
        boost::shared_ptr<UnicodeCharacter> ret;
        return ret;
    }

    UniFontLoader::MapType::iterator iter = border ? cacheBorder_.find(character) : cache_.find(character);
    if (iter != (border ? cacheBorder_.end() : cache_.end())) {
        boost::shared_ptr<UnicodeCharacter> ret = iter->second;
        return ret;
    }

    stream_.seekg(offsets_[character], std::ios_base::beg);
    uint8_t charHeader[4];
    stream_.read(reinterpret_cast<char*>(charHeader), 4);

    boost::shared_ptr<UnicodeCharacter> ret(new UnicodeCharacter(charHeader[0], charHeader[1], charHeader[2], charHeader[3], border));

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
                ret->data_[y * ret->width_ + x] = UnicodeCharacter::LETTER;
            }

            if (bitIndex == 0) {
                bitIndex = 7;
                ++scanlinePtr;
            } else {
                bitIndex--;
            }

        }
    }

    if (border) {
        for (unsigned int y = BORDER_WIDTH; y < ret->height_ - BORDER_WIDTH; ++y) {
            for (unsigned int x = BORDER_WIDTH; x < ret->width_ -BORDER_WIDTH; ++x) {
                if (ret->data_[y * ret->width_ + x] == UnicodeCharacter::LETTER) {

                    for (int i = -BORDER_WIDTH; i <= BORDER_WIDTH; ++i) {
                        for (int j = -BORDER_WIDTH; j <= BORDER_WIDTH; ++j) {
                            int idx = i;
                            idx += y;
                            idx *= ret->width_;
                            idx += j;
                            idx += x;
                            if (ret->data_[idx] != UnicodeCharacter::LETTER) {
                                ret->data_[idx] = UnicodeCharacter::BORDER;
                            }
                        }
                    }
                }
            }
        }

        cacheBorder_[character] = ret;
    } else {
        cache_[character] = ret;
    }


    //LOG_DEBUG << "read char" << std::endl;
    //for (unsigned int y = 0; y < ret->height_; ++y) {
        //for (unsigned int x = 0; x < ret->width_; ++x) {
            //if (ret->data_[y * ret->width_ + x] == UnicodeCharacter::LETTER) {
                //printf("o");
            //} else {
                //printf(" ");
            //}
        //}
        //printf("\n");
    //}

    //LOG_DEBUG << "end read char" << std::endl << std::endl;


    return ret;
}

boost::shared_ptr<ui::Texture> UniFontLoader::getText(const UnicodeString& text, bool border, unsigned int maxWidth, uint32_t color) {
    boost::shared_ptr<ui::Texture> tex(new ui::Texture);

    // calculate size
    unsigned int width = 0;

    unsigned int lineCount = 1;
    unsigned int curWidth = 0;
    unsigned int curIndex = 0;
    unsigned int lastBlank = 0;
    unsigned int widthSinceLastBlank = 0;
    std::list<unsigned int> lineBreakIndices;

    StringCharacterIterator iter(text);

    while (iter.hasNext()) {
        unsigned int charCode = iter.nextPostInc();
        //LOG_DEBUG << "iter char=" << charCode << " (" << (char)charCode << ")" << std::endl;
        boost::shared_ptr<UnicodeCharacter> curChar = getCharacter(charCode, border);

        unsigned int curCharWidth;

        if (charCode == ' ') {
            lastBlank = curIndex;
            widthSinceLastBlank = 0;
            curCharWidth = 5;
        } else if (!curChar) {
            LOG_DEBUG << "Trying to render invalid char code " << charCode << std::endl;
            continue;
        } else {
            curCharWidth = curChar->getTotalWidth();
        }

        if (curWidth + curCharWidth >= maxWidth) {
            // word exceeds line width
            width = std::max(width, curWidth);

            if (lastBlank != 0) {
                // move word to next line
                lineBreakIndices.push_back(lastBlank);
                curWidth = widthSinceLastBlank;
            } else {
                // word too long for one line, break here
                lineBreakIndices.push_back(curIndex);
                curWidth = 0;
            }
            ++lineCount;
            widthSinceLastBlank = 0;

        } else {
            if (charCode != ' ') {
                widthSinceLastBlank += curCharWidth;
            }
            curWidth += curCharWidth + 2;
        }

        ++curIndex;
    }

    unsigned int height = lineCount * (maxHeight_ + 3);
    width = std::max(width, curWidth);

    LOG_DEBUG << "calculated width=" << width << "  height=" << height << std::endl;

    tex->initPixelBuffer(width, height);
    uint32_t* pixBufPtr = tex->getPixelBufferData();
    memset(pixBufPtr, 0, width * height * sizeof(uint32_t));


    StringCharacterIterator iter2(text);

    unsigned int curX = 0;
    unsigned int curY = 0;
    curIndex = 0;

    while (iter2.hasNext()) {
        if (!lineBreakIndices.empty() && lineBreakIndices.front() == curIndex) {
            curX = 0;
            curY += (maxHeight_ + 3);
            lineBreakIndices.pop_front();
        }

        unsigned int charCode = iter2.nextPostInc();
        boost::shared_ptr<UnicodeCharacter> curChar = getCharacter(charCode, border);

        if (!curChar) {
            //LOG_DEBUG << "Trying to render invalid char code (second) " << charCode << std::endl;
            continue;
        }

        for (unsigned int y = 0; y < curChar->height_; ++y) {
            for (unsigned int x = 0; x < curChar->width_; ++x) {
                switch(curChar->data_[y * curChar->width_ + x]) {
                case UnicodeCharacter::BORDER:
                    pixBufPtr[(curY + curChar->yOffset_ + y) * width + (curX + curChar->xOffset_ + x)] = 0x000000FF;
                    break;
                case UnicodeCharacter::LETTER:
                    pixBufPtr[(curY + curChar->yOffset_ + y) * width + (curX + curChar->xOffset_ + x)] = color;
                    break;
                }
            }
        }

        curX += curChar->getTotalWidth() + 2;
    }

    tex->setReadComplete();

    return tex;
}

#undef BORDER_WIDTH


}
}
