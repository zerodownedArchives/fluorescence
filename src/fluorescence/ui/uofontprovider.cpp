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

#include "uofontprovider.hpp"

#include <unicode/schriter.h>

#include <data/manager.hpp>
#include <data/unifontloader.hpp>
#include <data/huesloader.hpp>

#include "texture.hpp"


#include <ClanLib/Display/2D/draw.h>
#include <ClanLib/Core/Math/quad.h>


namespace fluo {
namespace ui {

UoFontProvider::UoFontProvider(unsigned int unifontId) : unifontId_(unifontId), historyIndex_(0), historySize_(0) {
    fontLoader_ = data::Manager::getUniFontLoader(unifontId);
    
    initFontMetrics();
}

CL_FontMetrics UoFontProvider::get_font_metrics() {
    return fontMetrics_;
}

void UoFontProvider::destroy() {
    fontLoader_.reset();
}

void UoFontProvider::draw_text(CL_GraphicContext& gc, float x, float y, const CL_StringRef& text, const CL_Colorf& color) {
    boost::shared_ptr<ui::Texture> tex = getTexture(gc, text, color);
    
    CL_Rectf rect(x, y - tex->getHeight(), CL_Sizef(tex->getWidth(), tex->getHeight()));    
    //LOG_DEBUG << "draw_text: at " << rect << ": " << text.c_str() << std::endl;
    
    CL_Draw::texture(gc, *tex->getTexture(), CL_Quadf(rect));
}

CL_Size UoFontProvider::get_text_size(CL_GraphicContext& gc, const CL_StringRef& text) {
    // calculate size
    unsigned int width = borderWidth_ * 2;

    for (CL_String::size_type p = 0; p < text.length(); p++)
	{
        unsigned int charCode = text[p];
        boost::shared_ptr<data::UnicodeCharacter> curChar = fontLoader_->getCharacter(charCode);
        
        if (!curChar) {
            LOG_DEBUG << "Trying to render invalid char code " << charCode << std::endl;
            continue;
        }

        unsigned int curCharWidth = curChar->getTotalWidth();

        width += curCharWidth + spaceWidth_;
    }

    unsigned int height = fontLoader_->getMaxHeight() + borderWidth_*2;
    
    //LOG_DEBUG << "uofontprovider calc size: width=" << width << " height=" << height << " loader height=" << fontLoader_->getMaxHeight() << std::endl;
    
    return CL_Size(width, height);
}

int UoFontProvider::get_character_index(CL_GraphicContext& gc, const CL_String& text, const CL_Point& point) {
    //LOG_DEBUG << "get_char_index: " << point.x << "/" << point.y << std::endl;
    
    int curX = 0;
	int curY = 0;

	CL_FontMetrics fm = get_font_metrics();
	int lineHeight = fontLoader_->getMaxHeight() + fm.get_external_leading();

    int lineIdx = point.y / lineHeight; // char should be in this line
    int targetYMin = lineHeight * lineIdx;
    int targetYMax = targetYMin + lineHeight;
    
    CL_String::size_type stringLen = text.length();
    for (CL_String::size_type p = 0; p < stringLen; p++) {
        if (text[p] == '\n') {
            curY += lineHeight;
            continue;
        }
        if (curY >= targetYMin && curY < targetYMax) {
            // this is the line we really need to look at
            boost::shared_ptr<data::UnicodeCharacter> curChar = fontLoader_->getCharacter(text[p]);
            if (point.x >= curX && point.x < (curX + curChar->getTotalWidth())) {
                return p;
            } else {
                curX += curChar->getTotalWidth();
            }
        } else if (curY >= targetYMax) {
            // target was the last character on the previous line
            return (p-1);
        }
    }
    
	return -1;	// Not found
}

void UoFontProvider::initFontMetrics() {
    unsigned int height, ascent, descent, avgWidth, maxWidth;
    height = ascent = descent = avgWidth = maxWidth = 0;
    
    boost::shared_ptr<data::UnicodeCharacter> charM = fontLoader_->getCharacter('M');
    if (charM) {
        height = charM->getTotalHeight() + borderWidth_ * 2;
    }
    
    boost::shared_ptr<data::UnicodeCharacter> charARing = fontLoader_->getCharacter(0xc5); // Å
    if (charARing) {
        ascent = charARing->getTotalHeight() + borderWidth_ * 2;
    }
    
    boost::shared_ptr<data::UnicodeCharacter> charg = fontLoader_->getCharacter('g');
    if (charg) {
        descent = borderWidth_ + charg->getTotalHeight() - charM->getTotalHeight();
        avgWidth = charg->getTotalWidth() + borderWidth_ * 2;
    }
    
    boost::shared_ptr<data::UnicodeCharacter> charW = fontLoader_->getCharacter('W');
    if (charW) {
        maxWidth = charW->getTotalWidth() + borderWidth_ * 2;
    }
    
    // LOG_DEBUG << "font metrics: height=" << height << " ascent=" << ascent << " descent=" << descent << std::endl;
    
    
    fontMetrics_ = CL_FontMetrics(
		height,		// height
		ascent,		// ascent
		descent,			// descent
		ascent - height,			// internal_leading
		1,			// external_leading
		avgWidth, // average_character_width
		maxWidth, // max_character_width
		0.0f,		// weight
		0.0f,		// overhang
		96.0f,		// digitized_aspect_x
		96.0f,		// digitized_aspect_y
		false,		// italic
		false,		// underline
		false,		// struck_out
		false);		// fixed_pitch
}

void UoFontProvider::applyBorder(boost::shared_ptr<CL_PixelBuffer> pxBuf, const CL_Colorf& clcolor, unsigned int borderWidth, const CL_Colorf& clborderColor) {
    uint32_t color = clToUintColor(clcolor);
    uint32_t borderColor = clToUintColor(clborderColor);
    
    // dirty fix to avoid a filled rectangle... 
    if (color == borderColor) {
        borderColor = color - 1;
    }
    
    unsigned int width = pxBuf->get_width();
    unsigned int height = pxBuf->get_height();

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

boost::shared_ptr<ui::Texture> UoFontProvider::getTexture(CL_GraphicContext& gc, const CL_StringRef& cltext, const CL_Colorf& clcolor, unsigned int borderWidth, const CL_Colorf& clborderColor) {
    UnicodeString text(cltext.c_str());
    // check history first!
    boost::shared_ptr<ui::Texture> tex = findInHistory(text);
    if (tex) {
        return tex;
    }
    
    //LOG_DEBUG << "Texture not found in history" << std::endl;
    
    uint32_t color = clToUintColor(clcolor);
    
    CL_Size texSize = get_text_size(gc, cltext);
    unsigned int width = texSize.width;
    unsigned int height = texSize.height;

    tex.reset(new ui::Texture());
    tex->initPixelBuffer(width, height);
    uint32_t* pixBufPtr = tex->getPixelBufferData();

    unsigned int curX = borderWidth;
    unsigned int curY = borderWidth;

    StringCharacterIterator iter(text);
    while (iter.hasNext()) {
        unsigned int charCode = iter.nextPostInc();

        boost::shared_ptr<data::UnicodeCharacter> curChar = fontLoader_->getCharacter(charCode);

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

        curX += curChar->getTotalWidth() + spaceWidth_;
    }


    if (borderWidth > 0 && clborderColor != CL_Colorf::black) {
        applyBorder(tex->getPixelBuffer(), clcolor, borderWidth, clborderColor);
    }

    tex->setReadComplete();
    
    addToHistory(text, tex);

    return tex;
}

boost::shared_ptr<ui::Texture> UoFontProvider::findInHistory(const UnicodeString& str) {
    int32_t hash = str.hashCode();
    boost::shared_ptr<ui::Texture> ret;
    
    boost::mutex::scoped_lock myLock(historyMutex_);
    
    for (unsigned int i = 0; i < historySize_; ++i) {
        if (textureHistory_[i].first == hash) {
            ret = textureHistory_[i].second;
            break;
        }
    }
    
    return ret;
}

void UoFontProvider::addToHistory(const UnicodeString& str, boost::shared_ptr<ui::Texture> tex) {
    boost::mutex::scoped_lock myLock(historyMutex_);
    
    textureHistory_[historyIndex_].first = str.hashCode();
    textureHistory_[historyIndex_].second = tex;
    
    ++historyIndex_;
    historyIndex_ %= HISTORY_SIZE;
    
    if (historySize_ < HISTORY_SIZE) {
        ++historySize_;
    }
}

uint32_t UoFontProvider::clToUintColor(const CL_Colorf& clcolor) const {
    unsigned int r = (clcolor.get_red()) * 255;
    unsigned int g = (clcolor.get_green()) * 255;
    unsigned int b = (clcolor.get_blue()) * 255;
    unsigned int a = (clcolor.get_alpha()) * 255;
    
    uint32_t ret = (r & 0xFF) << 24;
    ret |= (g & 0xFF) << 16;
    ret |= (b & 0xFF) << 8;
    ret |= (a & 0xFF);
    return ret;
}

}
}
