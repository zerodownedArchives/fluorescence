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



#include "basebutton.hpp"

#include <ui/manager.hpp>
#include <ui/gumpmenu.hpp>
#include <ui/gumpactions.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace components {

BaseButton::BaseButton() : buttonType_(TYPE_UNDEFINED) {
}

void BaseButton::setServerButton(unsigned int buttonId) {
    buttonType_ = TYPE_SERVER;
    buttonId_ = buttonId;
}

void BaseButton::setPageButton(unsigned int pageId) {
    buttonType_ = TYPE_PAGE;
    pageId_ = pageId;
}

void BaseButton::setLocalButton(const UnicodeString& action) {
    buttonType_ = TYPE_LOCAL;
    action_ = action;
}

void BaseButton::handleClick() {
    switch (buttonType_) {
    case TYPE_UNDEFINED:
        LOG_ERROR << "Button type not defined" << std::endl;
        break;
    case TYPE_PAGE:
        onClickPage();
        break;
    case TYPE_SERVER:
        onClickServer();
        break;
    case TYPE_LOCAL:
        onClickLocal();
        break;
    }
}

void BaseButton::onClickPage() {
    GumpMenu* gump = getTopLevelMenu();
    if (gump) {
        gump->activatePage(pageId_);
    } else {
        LOG_ERROR << "BaseButton inside something other than GumpMenu" << std::endl;
    }
}

void BaseButton::onClickServer() {
    GumpMenu* gump = getTopLevelMenu();
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

void BaseButton::onClickLocal() {
    GumpActions::doAction(this);
}

void BaseButton::setParameter(const UnicodeString& value, unsigned int index) {
    if (index >= MAX_PARAMETER_COUNT) {
        LOG_INFO << "LocalButton::setParameter index too high: " << index << ": " << value << std::endl;
    } else {
        parameter_[index] = value;
    }
}

const UnicodeString* BaseButton::getParameterPtr() const {
    return parameter_;
}

const UnicodeString& BaseButton::getAction() const {
    return action_;
}

}
}
}
