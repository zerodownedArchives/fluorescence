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

#include "button.hpp"

#include <ClanLib/Display/Window/keys.h>

#include <data/manager.hpp>
#include <data/huesloader.hpp>
#include <ui/gumpmenu.hpp>
#include <ui/manager.hpp>
#include <ui/python/scriptloader.hpp>

#include "scrollarea.hpp"

namespace fluo {
namespace ui {
namespace components {

Button::Button(CL_GUIComponent* parent) : Image(parent),
        mouseOver_(false), mouseDown_(false),
        type_(SERVER), buttonId_(0), pageId_(0) {

    func_input_pressed().set(this, &Button::onInputPressed);
    func_input_released().set(this, &Button::onInputReleased);
    func_pointer_enter().set(this, &Button::onPointerEnter);
    func_pointer_exit().set(this, &Button::onPointerExit);

    set_double_click_enabled(false);

    set_type_name("button");
}

bool Button::onInputReleased(const CL_InputEvent & e) {
    if (e.id == CL_MOUSE_LEFT) {
        mouseDown_ = false;
        updateState();
        handleClick();
        return true;
    } else {
        return false;
    }
}

bool Button::onInputPressed(const CL_InputEvent & e) {
    if (e.id == CL_MOUSE_LEFT) {
        mouseDown_ = true;
        updateState();
        return true;
    } else {
        return false;
    }
}

bool Button::onPointerEnter() {
    mouseOver_ = true;
    updateState();

    return true;
}

bool Button::onPointerExit() {
    mouseOver_ = false;
    mouseDown_ = false;
    updateState();

    return true;
}

void Button::updateState() {
    if (mouseDown_) {
        setCurrentState("mousedown");
    } else {
        if (mouseOver_) {
            setCurrentState("mouseover");
        } else {
            setCurrentState("normal");
        }
    }
}

ImageState* Button::getStateMouseOver() {
    return getState("mouseover");
}

ImageState* Button::getStateMouseDown() {
    return getState("mousedown");
}

Button::ButtonType Button::getButtonType() const {
    return type_;
}

void Button::setButtonType(Button::ButtonType type) {
    type_ = type;
}

unsigned int Button::getButtonId() const {
    return buttonId_;
}

void Button::setButtonId(unsigned int id) {
    buttonId_ = id;
    type_ = SERVER;
}

unsigned int Button::getPage() const {
    return pageId_;
}

void Button::setPage(unsigned int id) {
    pageId_ = id;
    type_ = PAGE;
}

void Button::handleClick() {
    switch (type_) {
    case PAGE:
        onClickPage();
        break;
    case SERVER:
        onClickServer();
        break;
    case PYTHON:
        onClickPython();
        break;
    }
}

void Button::onClickPage() {
    GumpMenu* gump = getGumpMenu();
    if (gump) {
        gump->activatePage(pageId_);
    } else {
        LOG_ERROR << "BaseButton inside something other than GumpMenu" << std::endl;
    }
}

void Button::onClickServer() {
    GumpMenu* gump = getGumpMenu();
    if (gump) {
        if (buttonId_ == 0) {
            ui::Manager::getSingleton()->closeGumpMenu(gump);
        } else {
            // send gump reply to server
            gump->sendReply(buttonId_);
        }
    } else {
        LOG_ERROR << "BaseButton inside something other than GumpMenu" << std::endl;
    }
}

void Button::onClickPython() {
    if (pyClickCallback_) {
        try {
            pyClickCallback_(boost::python::ptr(this));
        } catch (boost::python::error_already_set const&) {
            ui::Manager::getPythonLoader()->logError();
        }
    }
}

boost::python::object Button::getPyClickCallback() const {
    return pyClickCallback_;
}

void Button::setPyClickCallback(boost::python::object& obj) {
    pyClickCallback_ = obj;
    type_ = PYTHON;
}

}
}
}
