
#include "fontengine.hpp"

#include <ClanLib/Display/Font/font_system.h>
#include <unicode/schriter.h>

#include "texture.hpp"
#include "manager.hpp"

#include <misc/log.hpp>

#include <data/manager.hpp>
#include <data/unifontloader.hpp>

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

boost::shared_ptr<ui::Texture> FontEngine::getDefaultTexture(const UnicodeString& text, unsigned int maxWidth, uint32_t color, unsigned int borderWidth, uint32_t borderColor) {
    if (useDefaultUniFont_) {
        return getUniFontTexture(defaultUniFont_, text, maxWidth, color, borderWidth, borderColor);
    } else {
        return getFontTexture(defaultFont_, text, maxWidth, color, borderWidth, borderColor);
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
        //LOG_DEBUG << "iter char=" << charCode << " (" << (char)charCode << ")" << std::endl;
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

        if (curWidth + curCharWidth >= maxWidth) {
            // word exceeds line width
            width = (std::min)(width, curWidth);

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
            curWidth += curCharWidth + uniCharSpacing_;
        }

        ++curIndex;
    }

    height = lineCount * (fontLoader->getMaxHeight() + uniLineSpacing_) + borderWidth*2;
    width = (std::max)(width, curWidth);

    //LOG_DEBUG << "calculated width=" << width << "  height=" << height << std::endl;
    //LOG_DEBUG << "line breaks: " << lineBreakIndices.front() << std::endl;
}

boost::shared_ptr<ui::Texture> FontEngine::getUniFontTexture(unsigned int uniFontId, const UnicodeString& text, unsigned int maxWidth, uint32_t color, unsigned int borderWidth, uint32_t borderColor) {
    std::list<unsigned int> lineBreakIndices;
    unsigned int width;
    unsigned int height;

    calculateSizeAndLinebreaks(uniFontId, text, maxWidth, borderWidth, width, height, lineBreakIndices);


    boost::shared_ptr<data::UniFontLoader> fontLoader = data::Manager::getUniFontLoader(uniFontId);

    boost::shared_ptr<ui::Texture> tex(new ui::Texture);
    tex->initPixelBuffer(width, height);
    uint32_t* pixBufPtr = tex->getPixelBufferData();


    unsigned int curX = 0;
    unsigned int curY = 0;
    unsigned int curIndex = 0;

    if (borderWidth > 0) {
        curX += borderWidth;
        curY += borderWidth;
    }

    StringCharacterIterator iter(text);
    while (iter.hasNext()) {
        unsigned int charCode = iter.nextPostInc();

        if (!lineBreakIndices.empty() && lineBreakIndices.front() == curIndex) {
            curX = 0;
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
        applyBorder(tex, color, borderWidth, borderColor);
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
    //unsigned int curIndex = 0;
    //unsigned int lastBlank = 0;
    //unsigned int widthSinceLastBlank = 0;

    int lastSpaceIdx = 0;
    int lastNewLineIdx = 0;
    int curSpaceIdx = 0;

    LOG_DEBUG << "loop start" << std::endl;
    while (curSpaceIdx != -1) {
        curSpaceIdx = text.indexOf(' ', lastSpaceIdx + 1);

        // if space is found, use substring until space. if not, use rest of the string
        unsigned int strLen = curSpaceIdx != -1 ? curSpaceIdx - lastNewLineIdx : text.length() - lastNewLineIdx;
        UnicodeString lineTest(text, lastNewLineIdx, strLen);

        CL_Size lineSize = font.get_text_size(gc, StringConverter::toUtf8String(lineTest));
        curWidth = lineSize.width;

        // there seems to be some difference in the font size calculated by clanlib and the actual pixels required ^^
        curWidth *= 1.1f;
        LOG_DEBUG << "cur line=\"" << lineTest << "\" curWidth " << curWidth << std::endl;

        if (curWidth < maxWidth) {
            // this substring fits the line
            lastSpaceIdx = curSpaceIdx;
            width = (std::max)(width, curWidth);
            LOG_DEBUG << "fits line, width=" << width << std::endl;
        } else {
            // substring too long
            LOG_DEBUG << "need line break" << std::endl;
            // if there was a space before, use it as line break
            if (lastSpaceIdx != lastNewLineIdx - 1) {
                textWithBreaks.setCharAt(lastSpaceIdx, '\n');
                lastNewLineIdx = lastSpaceIdx + 1;
            } else {
                // long word needs to be split

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

boost::shared_ptr<ui::Texture> FontEngine::getFontTexture(CL_Font& font, const UnicodeString& text, unsigned int maxWidth, uint32_t color, unsigned int borderWidth, uint32_t borderColor) {
    CL_GraphicContext& gc = ui::Manager::getSingleton()->getGraphicContext();

    CL_FrameBuffer origBuffer = gc.get_write_frame_buffer();

    boost::shared_ptr<ui::Texture> fontTexture(new ui::Texture);
    // TODO: calculate size and line breaks
    unsigned int width;
    unsigned int height;
    UnicodeString textWithBreaks = calculateSizeAndLinebreaks(font, text, maxWidth, borderWidth, width, height);

    fontTexture->initPixelBuffer(width, height);
    CL_FrameBuffer fb(gc);
    fb.attach_color_buffer(0, *fontTexture->getTexture(false));

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

    // applying the border has to be done after resetting the frame buffer!
    if (borderWidth > 0) {
        applyBorder(fontTexture, color, borderWidth, borderColor);
    }

    fontTexture->setReadComplete();

    return fontTexture;
}

void FontEngine::applyBorder(boost::shared_ptr<ui::Texture> tex, uint32_t color, unsigned int borderWidth, uint32_t borderColor) {
    unsigned int width = tex->getWidth();
    unsigned int height = tex->getHeight();

    // this is a bit complicated, but as system fonts are directly rendered to a texture there is no easy way around
    CL_PixelBuffer pixBuf = tex->getTexture()->get_pixeldata();
    uint32_t* pixBufPtr = (uint32_t*)pixBuf.get_data();
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

    tex->getTexture()->set_image(pixBuf);
}

}
}
