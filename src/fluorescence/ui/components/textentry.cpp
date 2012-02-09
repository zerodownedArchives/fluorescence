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

#include "textentry.hpp"

#include <ClanLib/Display/2D/draw.h>
#include <ClanLib/GUI/gui_manager.h>

#include <ui/manager.hpp>
#include <misc/log.hpp>
#include <data/manager.hpp>
#include <data/huesloader.hpp>

namespace fluo {
namespace ui {
namespace components {

TextEntry::TextEntry(CL_GUIComponent* parent) : LineEdit(parent) {
}

void TextEntry::setFont(const CL_FontDescription& desc) {
    font_ = ui::Manager::getGuiManager()->get_registered_font(desc);
    if (font_.is_null()) {
        font_ = CL_Font_System(ui::Manager::getSingleton()->getGraphicContext(), desc);
    }
}

CL_Font TextEntry::get_font_override() const {
    return font_;
}

CL_StringRef TextEntry::get_css_override(const CL_GUIThemePart& part, const CL_String& name) const {
    //LOG_DEBUG << "css override: " << name.c_str() << std::endl;
    if (name == "bg-image") {
        return "";
    } else if (name == "bg-color") {
        CL_String elemName = part.get_element_name();
        LOG_DEBUG << "bg-color elem " << part.get_element_name().c_str() << "  ::: index=" << (int)part.get_element_name().find("selected") << std::endl;
        if ((int)elemName.find("selection") > 0) {
            return "#99999999";
        } else if ((int)elemName.find("cursor") > 0) {
            return textColor_;
        } else {
            return "transparent";
        }
    } else if (name == "text-color") {
        return textColor_;
    }
    
    return CL_StringRef();
}

void TextEntry::setTextHue(unsigned int hue) {
    if (hue != 0) {
        textColor_ = StringConverter::toUtf8String(data::Manager::getHuesLoader()->getFontRgbString(hue));
    } else {
        textColor_ = "#000000";
    }
}

}
}
}

