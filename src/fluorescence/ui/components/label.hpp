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


#ifndef FLUO_UI_COMPONENTS_LABEL_HPP
#define FLUO_UI_COMPONENTS_LABEL_HPP

#include <ui/gumpcomponent.hpp>

#include <ClanLib/Display/2D/span_layout.h>
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/Display/Font/font_description.h>

#include <misc/string.hpp>

namespace fluo {
namespace ui {
namespace components {

class Label : public GumpComponent {
public:
    struct Alignment {
        enum {
            LEFT = CL_SpanAlign::cl_left,
            CENTER = CL_SpanAlign::cl_center,
            RIGHT = CL_SpanAlign::cl_right,
        };
    };

    Label(CL_GUIComponent* parent);

    void setText(const UnicodeString& string);
    UnicodeString getText();

    void setAlignment(unsigned int align);
    void setFont(const UnicodeString& name, unsigned int height);
    void setColor(const CL_Colorf& color);
    void setHue(unsigned int hue);

    void setHtmlText(const UnicodeString& string);

protected:
    CL_SpanLayout span_;
    CL_FontDescription fontDesc_;
    CL_Font cachedFont_;
    CL_Colorf fontColor_;

private:
    UnicodeString text_;
    unsigned int alignment_;

    void adjustSize();

    void onRender(CL_GraphicContext& gc, const CL_Rect& update_rect);

    bool hasScrollareaParent_;
};

}
}
}

#endif
