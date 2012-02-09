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


#ifndef FLUO_UI_COMPONENTS_TEXTENTRY_HPP
#define FLUO_UI_COMPONENTS_TEXTENTRY_HPP

#include "lineedit.hpp"

#include <ClanLib/Display/Font/font.h>

namespace fluo {
namespace ui {
namespace components {
    
class TextEntry : public LineEdit {
public:
    TextEntry(CL_GUIComponent* parent);
    
    void render(CL_GraphicContext& gc, const CL_Rect& updateRect);
    
    void setFont(const CL_FontDescription& fontDesc);
    void setTextHue(unsigned int hue);
    
    virtual CL_Font get_font_override() const;
    virtual CL_StringRef get_css_override(const CL_GUIThemePart& part, const CL_String& name) const;
    
private:
    CL_Font font_;
    CL_String textColor_;
};

}
}
}

#endif

