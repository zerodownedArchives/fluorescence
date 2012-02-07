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



#include "fontengine.hpp"

#include <ClanLib/Display/Font/font_system.h>
#include <unicode/schriter.h>

#include "texture.hpp"
#include "manager.hpp"

#include <misc/log.hpp>

#include <data/manager.hpp>
#include <data/unifontloader.hpp>
#include <data/huesloader.hpp>

namespace fluo {
namespace ui {

FontEngine::FontEngine(Config& config, CL_GraphicContext& gc) {
    const UnicodeString& fontName = config["/fluo/ui/gameview/font@name"].asString();
    if (fontName.startsWith("unifont")) {
        // use a font from unifontX.mul
        useDefaultUniFont_ = true;

        if (!config["/fluo/ui/gameview/font@size"].isDefault()) {
            LOG_INFO << "Setting font size for unifonts is not possible" << std::endl;
        }

        if (fontName == "unifont") {
            defaultUniFont_ = 0;
        } else {
            UnicodeString fontIdxStr(fontName, 7);
            defaultUniFont_ = StringConverter::toInt(fontIdxStr);
        }

    } else {
        useDefaultUniFont_ = false;

        CL_FontDescription font_desc;
        font_desc.set_typeface_name(StringConverter::toUtf8String(fontName));
        font_desc.set_height(config["/fluo/ui/gameview/font@size"].asInt());
        font_desc.set_subpixel(false);
        font_desc.set_anti_alias(false);
        defaultFont_ = CL_Font_System(gc, font_desc);
    }
}

boost::shared_ptr<ui::Texture> FontEngine::getDefaultTexture(const UnicodeString& text, unsigned int maxWidth, uint32_t color, bool useRgbColor, unsigned int borderWidth, uint32_t borderColor) {
    if (useDefaultUniFont_) {
        return getUniFontTexture(defaultUniFont_, text, maxWidth, color, useRgbColor, borderWidth, borderColor);
    } else {
        return getFontTexture(defaultFont_, text, maxWidth, color, useRgbColor, borderWidth, borderColor);
    }
}

void FontEngine::calculateSizeAndLinebreaks(unsigned int fontId, const UnicodeString& text, unsigned int maxWidth, unsigned int borderWidth,
        unsigned int& width, unsigned int& height, std::list<unsigned int>& lineBreakIndices) {
    boost::shared_ptr<data::UniFontLoader> fontLoader = data::Manager::getUniFontLoader(fontId);

    maxWidth -= borderWidth * 2;

    // calculate size
    unsigned int lineCount = 1;
    unsigned int curWidth = 0;
    unsigned int curIndex = 0;
    unsigned int lastBlank = 0;
    unsigned int widthSinceLastBlank = 0;

    StringCharacterIterator iter(text);

    while (iter.hasNext()) {
        unsigned int charCode = iter.nextPostInc();
        boost::shared_ptr<data::UnicodeCharacter> curChar = fontLoader->getCharacter(charCode);

        unsigned int curCharWidth;

        if (charCode == ' ') {
            lastBlank = curIndex;
            widthSinceLastBlank = 0;
        } else if (!curChar) {
            LOG_DEBUG << "Trying to render invalid char code " << charCode << std::endl;
            continue;
        }

        curCharWidth = curChar->getTotalWidth();

        //LOG_DEBUG << "char " << (char)charCode << " (" << charCode << ") width=" << curCharWidth << " lineWidth=" << curWidth << " sinceBlank=" << widthSinceLastBlank << std::endl;

        if (curWidth + curCharWidth >= maxWidth) {
            // word exceeds line width

            if (lastBlank != 0) {
                // move word to next line
                lineBreakIndices.push_back(lastBlank);
                width = (std::max)(width, curWidth - widthSinceLastBlank);
                curWidth = widthSinceLastBlank;
            } else {
                // word too long for one line, break here
                //LOG_DEBUG << "word too long for line curWidth=" << curWidth << std::endl;
                lineBreakIndices.push_back(curIndex - 1);
                width = (std::max)(width, curWidth);
                curWidth = 0;
            }
            ++lineCount;
            widthSinceLastBlank = curWidth;
            lastBlank = 0;

        }

        if (charCode != ' ') {
            widthSinceLastBlank += curCharWidth + uniCharSpacing_;
        }
        curWidth += curCharWidth + uniCharSpacing_;
        ++curIndex;
    }

    height = lineCount * (fontLoader->getMaxHeight() + uniLineSpacing_) + borderWidth*2;
    width = (std::max)(width, curWidth) + borderWidth*2;

    // LOG_DEBUG << "calculated width=" << width << "  height=" << height << std::endl;
    //LOG_DEBUG << "line breaks[0]: " << lineBreakIndices.front() << std::endl;
}

boost::shared_ptr<ui::Texture> FontEngine::getUniFontTexture(unsigned int uniFontId, const UnicodeString& text, unsigned int maxWidth, uint32_t color, bool useRgbColor, unsigned int borderWidth, uint32_t borderColor) {
    std::list<unsigned int> lineBreakIndices;
    unsigned int width = 0;
    unsigned int height = 0;

    calculateSizeAndLinebreaks(uniFontId, text, maxWidth, borderWidth, width, height, lineBreakIndices);

    if (!useRgbColor) {
        // translate uo color to rgb
        boost::shared_ptr<data::HuesLoader> huesLoader = data::Manager::getHuesLoader();
        color = huesLoader->getFontRgbColor(color);
    }


    boost::shared_ptr<data::UniFontLoader> fontLoader = data::Manager::getUniFontLoader(uniFontId);

    boost::shared_ptr<ui::Texture> tex(new ui::Texture);
    tex->initPixelBuffer(width, height);
    uint32_t* pixBufPtr = tex->getPixelBufferData();


    unsigned int curX = borderWidth;
    unsigned int curY = borderWidth;
    unsigned int curIndex = 0;

    StringCharacterIterator iter(text);
    while (iter.hasNext()) {
        unsigned int charCode = iter.nextPostInc();

        if (!lineBreakIndices.empty() && lineBreakIndices.front() == curIndex) {
            curX = borderWidth;
            curY += (fontLoader->getMaxHeight() + uniLineSpacing_);
            lineBreakIndices.pop_front();

            if (charCode == ' ') {
                // don't start next line with a space
                continue;
            }
        }

        boost::shared_ptr<data::UnicodeCharacter> curChar = fontLoader->getCharacter(charCode);

        if (!curChar) {
            LOG_DEBUG << "Trying to render invalid char code " << charCode << std::endl;
            continue;
        }

        for (unsigned int y = 0; y < curChar->height_; ++y) {
            for (unsigned int x = 0; x < curChar->width_; ++x) {
                if (curChar->data_[y * curChar->width_ + x] == 1) {
                    pixBufPtr[(curY + curChar->yOffset_ + y) * width + (curX + curChar->xOffset_ + x)] = color;
                }
            }
        }

        curX += curChar->getTotalWidth() + uniCharSpacing_;
        ++curIndex;
    }


    if (borderWidth > 0) {
        applyBorder(tex->getPixelBuffer(), color, borderWidth, borderColor);
    }

    tex->setReadComplete();

    return tex;
}

UnicodeString FontEngine::calculateSizeAndLinebreaks(CL_Font& font, const UnicodeString& text, unsigned int maxWidth, unsigned int borderWidth,
        unsigned int& width, unsigned int& height) {
    CL_GraphicContext& gc = ui::Manager::getGraphicContext();

    UnicodeString textWithBreaks(text);

    maxWidth -= borderWidth * 2;

    // calculate size
    unsigned int lineCount = 1;
    unsigned int curWidth = 0;

    int lastSpaceIdx = 0;
    int lastNewLineIdx = 0;
    int curSpaceIdx = 0;

    //LOG_DEBUG << "loop start" << std::endl;
    while (curSpaceIdx != -1) {
        curSpaceIdx = text.indexOf(' ', lastSpaceIdx + 1);

        // if space is found, use substring until space. if not, use rest of the string
        unsigned int strLen = curSpaceIdx != -1 ? curSpaceIdx - lastNewLineIdx : text.length() - lastNewLineIdx;
        UnicodeString lineTest(text, lastNewLineIdx, strLen);

        CL_Size lineSize = font.get_text_size(gc, StringConverter::toUtf8String(lineTest));
        curWidth = lineSize.width;

        // there seems to be some difference in the font size calculated by clanlib and the actual pixels required ^^
        curWidth *= 1.1f;
        //LOG_DEBUG << "cur line=\"" << lineTest << "\" curWidth " << curWidth << std::endl;

        if (curWidth < maxWidth) {
            // this substring fits the line
            lastSpaceIdx = curSpaceIdx;
            width = (std::max)(width, curWidth);
            //LOG_DEBUG << "fits line, width=" << width << std::endl;
        } else {
            // substring too long
            //LOG_DEBUG << "need line break lastSpace=" << lastSpaceIdx << " lastNewLine=" << lastNewLineIdx << std::endl;
            // if there was a space before, use it as line break
            if (lastSpaceIdx != lastNewLineIdx - 1) {
                textWithBreaks.setCharAt(lastSpaceIdx, '\n');
                lastNewLineIdx = lastSpaceIdx + 1;

                // make sure the newly created line is checked also if the last word is overly long
                curSpaceIdx = 0;
            } else {
                // long word needs to be split
                //LOG_DEBUG << "long word " << lineTest << std::endl;
                while (curWidth >= maxWidth && lineTest.length() > 1) {
                    lineTest.remove(lineTest.length() - 1, 1);
                    CL_Size lineSize = font.get_text_size(gc, StringConverter::toUtf8String(lineTest));
                    curWidth = lineSize.width;
                    curWidth *= 1.1f;
                }

                //LOG_DEBUG << "line reduced to " << lineTest << std::endl;

                lastNewLineIdx += lineTest.length();
                lastSpaceIdx = lastNewLineIdx - 1;
                textWithBreaks.insert(lastNewLineIdx, '\n');

                // make sure the newly created line is checked also if the last part of the split word is overly long
                curSpaceIdx = 0;
            }

            ++lineCount;
        }
    }

    CL_Size size = font.get_text_size(gc, StringConverter::toUtf8String(textWithBreaks));
    height = size.height + borderWidth * 2;
    width = size.width + borderWidth * 2;
    width *= 1.1f;

    //LOG_DEBUG << "calculated width=" << width << "  height=" << height << std::endl;
    //LOG_DEBUG << "font height=" << font.get_font_metrics().get_height()
                //<< " ascent=" << font.get_font_metrics().get_ascent()
                //<< " descent=" << font.get_font_metrics().get_descent()
                //<< " internal leading=" << font.get_font_metrics().get_internal_leading()
                //<< " external leading" << font.get_font_metrics().get_external_leading()
                //<< std::endl;

    return textWithBreaks;
}

boost::shared_ptr<ui::Texture> FontEngine::getFontTexture(CL_Font& font, const UnicodeString& text, unsigned int maxWidth, uint32_t color, bool useRgbColor, unsigned int borderWidth, uint32_t borderColor) {
    CL_GraphicContext& gc = ui::Manager::getSingleton()->getGraphicContext();

    CL_FrameBuffer origBuffer = gc.get_write_frame_buffer();


    unsigned int width = 0;
    unsigned int height = 0;
    UnicodeString textWithBreaks = calculateSizeAndLinebreaks(font, text, maxWidth, borderWidth, width, height);

    if (!useRgbColor) {
        // translate uo color to rgb
        boost::shared_ptr<data::HuesLoader> huesLoader = data::Manager::getHuesLoader();
        color = huesLoader->getFontRgbColor(color);
    }

    CL_Texture tempTex(gc, width, height, cl_rgba8);
    CL_FrameBuffer fb(gc);
    fb.attach_color_buffer(0, tempTex);

    gc.set_frame_buffer(fb);

    gc.clear(CL_Colorf(0.f, 0.f, 0.f, 0.f));
    // when drawing text, (0,0) is not the top left corner but the vertical center of the first line
    CL_FontMetrics metrics = font.get_font_metrics();
    unsigned int vertOffset = metrics.get_ascent() - metrics.get_internal_leading();

    int r = color >> 24;
    int g = (color >> 16) & 0xFF;
    int b = (color >> 8) & 0xFF;
    int a = color & 0xFF;
    font.draw_text(gc, 0, vertOffset + borderWidth, StringConverter::toUtf8String(textWithBreaks), CL_Colorf(r, g, b, a));

    gc.set_frame_buffer(origBuffer);

    boost::shared_ptr<ui::Texture> fontTexture(new ui::Texture);
    fontTexture->initPixelBuffer(width, height);
    memcpy(fontTexture->getPixelBufferData(), tempTex.get_pixeldata().get_data(), width * height * 4);

    // applying the border has to be done after resetting the frame buffer!
    if (borderWidth > 0) {
        applyBorder(fontTexture->getPixelBuffer(), color, borderWidth, borderColor);
    }

    fontTexture->setReadComplete();

    return fontTexture;
}

void FontEngine::applyBorder(boost::shared_ptr<CL_PixelBuffer> pxBuf, uint32_t color, unsigned int borderWidth, uint32_t borderColor) {
    unsigned int width = pxBuf->get_width();
    unsigned int height = pxBuf->get_height();

    // this is a bit complicated, but as system fonts are directly rendered to a texture there is no easy way around
    uint32_t* pixBufPtr = (uint32_t*)pxBuf->get_data();
    int iBorderWidth = borderWidth;

    for (unsigned int y = borderWidth; y < height - borderWidth; ++y) {
        for (unsigned int x = borderWidth; x < width -borderWidth; ++x) {
            unsigned int curIndex = y * width + x;
            if (pixBufPtr[curIndex] == color) {
                for (int i = -iBorderWidth; i <= iBorderWidth; ++i) {
                    for (int j = -iBorderWidth; j <= iBorderWidth; ++j) {
                        int idx = (i+y) * width + j+x;
                        if (pixBufPtr[idx] != color) {
                            pixBufPtr[idx] = borderColor;
                        }
                    }
                }
            }
        }
    }
}

}
}
