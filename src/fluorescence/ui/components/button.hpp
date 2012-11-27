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


#ifndef FLUO_UI_COMPONENTS_BUTTON_HPP
#define FLUO_UI_COMPONENTS_BUTTON_HPP

#include <boost/shared_ptr.hpp>
#include <ClanLib/Display/Window/input_event.h>
#include <ClanLib/Display/2D/span_layout.h>
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/Display/Font/font_description.h>
#include <boost/python/object.hpp>

#include <misc/string.hpp>
#include <ui/enums.hpp>

#include "image.hpp"

namespace fluo {
namespace ui {
namespace components {

class Button : public Image {
public:
    Button(CL_GUIComponent* parent);

    void setFont(const UnicodeString& name, unsigned int height);
    void setFontAlignment(unsigned int align);
    void setText(unsigned int index, const UnicodeString& text);
    void setFontColor(unsigned int index, const CL_Colorf& color);
    void setFontHue(unsigned int index, unsigned int hue);

    ButtonType getButtonType() const;
    void setButtonType(ButtonType type);

    unsigned int getButtonId() const;
    void setButtonId(unsigned int id);

    unsigned int getPage() const;
    void setPage(unsigned int id);

    // python specific api
    boost::python::object getPyClickCallback() const;
    void setPyClickCallback(boost::python::object& obj);

private:
    bool onInputPressed(const CL_InputEvent & e);
    bool onInputReleased(const CL_InputEvent & e);

    bool onPointerEnter();
    bool onPointerExit();

    bool mouseOver_;
    bool mouseDown_;
    void updateState();

    ButtonType type_;

    // for server button
    unsigned int buttonId_;

    // for page button
    unsigned int pageId_;

    void handleClick();
    void onClickPage();
    void onClickServer();
    void onClickPython();

    boost::python::object pyClickCallback_;
};

}
}
}

#endif
