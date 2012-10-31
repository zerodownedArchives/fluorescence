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
#include <ui/enums.hpp>

namespace fluo {
namespace ui {
namespace components {

class Label : public GumpComponent {
public:
    Label(CL_GUIComponent* parent);

    void setText(const UnicodeString& string);
    UnicodeString getText();

    void setFont(const UnicodeString& name, unsigned int height);
    void setFontB(const UnicodeString& name, unsigned int height, bool border);

    void setRgba(float r, float g, float b, float a);
    void setRgba(float r, float g, float b);
    void setRgba(const CL_Colorf& color);
    CL_Colorf getRgba() const;

    void setHue(unsigned int hue);

    void setHtmlText(const UnicodeString& string);

    void setVAlign(VAlign align);
    VAlign getVAlign() const;

    void setHAlign(HAlign align);
    HAlign getHAlign() const;

    virtual void setAutoResize(bool value);

    // python specific api
    boost::python::tuple pyGetRgba() const;
    void pySetRgba(const boost::python::tuple& rgba);

protected:
    CL_SpanLayout span_;
    bool overrideGumpFont_;
    CL_FontDescription fontDesc_;
    CL_Font cachedFont_;
    CL_Colorf rgba_;

private:
    UnicodeString text_;
    VAlign vAlign_;
    HAlign hAlign_;

    bool autoResize_;
    void layout();

    void onRender(CL_GraphicContext& gc, const CL_Rect& update_rect);

    bool hasScrollareaParent_;
};

}
}
}

#endif
