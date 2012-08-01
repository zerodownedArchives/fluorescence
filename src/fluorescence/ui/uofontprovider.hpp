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


#ifndef FLUO_UI_UOFONTPROVIDER_HPP
#define FLUO_UI_UOFONTPROVIDER_HPP

#include <ClanLib/Core/Text/string_types.h>
#include <ClanLib/Display/TargetProviders/font_provider.h>
#include <ClanLib/Display/Font/font_metrics.h>
#include <ClanLib/Display/Image/pixel_buffer.h>
#include <ClanLib/Display/2D/color.h>

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include <misc/string.hpp>

namespace fluo {

namespace data {
class UniFontLoader;
}

namespace ui {

class Texture;

class UoFontProvider : public CL_FontProvider {
public:
    UoFontProvider(unsigned int unifontId);

    virtual CL_FontMetrics get_font_metrics();

public:
	virtual void destroy();
	virtual void draw_text(CL_GraphicContext &gc, float x, float y, const CL_StringRef &text, const CL_Colorf &color);
	virtual CL_Size get_text_size(CL_GraphicContext &gc, const CL_StringRef &text);
	virtual int get_character_index(CL_GraphicContext &gc, const CL_String &text, const CL_Point &point);

private:
    unsigned int unifontId_;
    static const unsigned int spaceWidth_ = 1;
    static const unsigned int borderWidth_ = 1;

    boost::shared_ptr<data::UniFontLoader> fontLoader_;

    void initFontMetrics();
    CL_FontMetrics fontMetrics_;


    boost::shared_ptr<ui::Texture> getTexture(CL_GraphicContext& gc, const CL_StringRef& text, const CL_Colorf& clcolor, unsigned int borderWidth = 1, const CL_Colorf& clborderColor = CL_Colorf::black);
    void applyBorder(CL_PixelBuffer pxBuf, const CL_Colorf& clcolor, unsigned int borderWidth, const CL_Colorf& clborderColor);

    boost::shared_ptr<ui::Texture> findInHistory(const UnicodeString& str);
    void addToHistory(const UnicodeString& str, boost::shared_ptr<ui::Texture> tex);

    uint32_t clToUintColor(const CL_Colorf& clcolor) const;
};

}
}

#endif

