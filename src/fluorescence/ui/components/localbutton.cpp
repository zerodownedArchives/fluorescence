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



#include "localbutton.hpp"

#include <misc/log.hpp>

#include <ui/gumpactions.hpp>

namespace fluo {
namespace ui {
namespace components {

LocalButton::LocalButton(CL_GUIComponent* parent, const UnicodeString& action) : BaseButton(parent),
        action_(action) {
}

void LocalButton::setAction(const UnicodeString& value) {
    action_ = value;
}

const UnicodeString& LocalButton::getAction() const {
    return action_;
}

void LocalButton::setParameter(const UnicodeString& value, unsigned int index) {
    if (index >= MAX_PARAMETER_COUNT) {
        LOG_INFO << "LocalButton::setParameter index too high: " << index << std::endl;
        index = 0;
    }
    parameter_[index] = value;
}

const UnicodeString& LocalButton::getParameter(unsigned int index) const {
    if (index >= MAX_PARAMETER_COUNT) {
        LOG_INFO << "LocalButton::getParameter index too high: " << index << std::endl;
        index = 0;
    }
    return parameter_[index];
}

int LocalButton::getParameterInt(unsigned int index) const {
    if (index >= MAX_PARAMETER_COUNT) {
        LOG_INFO << "LocalButton::getParameterInt index too high: " << index << std::endl;
        index = 0;
    }
    std::string utfStr = StringConverter::toUtf8String(parameter_[index]);
    return atoi(utfStr.c_str());
}

const UnicodeString* LocalButton::getParameterPtr() const {
    return parameter_;
}

void LocalButton::onClicked(BaseButton* self) {
    GumpActions::doAction(this);
}

}
}
}
