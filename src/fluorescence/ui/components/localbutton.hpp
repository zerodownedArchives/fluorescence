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


#ifndef FLUO_UI_COMPONENTS_LOCALBUTTON_HPP
#define FLUO_UI_COMPONENTS_LOCALBUTTON_HPP

#include "basebutton.hpp"

#include <ui/gumpmenu.hpp>

namespace fluo {
namespace ui {
namespace components {

class LocalButton : public BaseButton {
public:
    static const unsigned int MAX_PARAMETER_COUNT = 5;

    LocalButton(CL_GUIComponent* parent, const UnicodeString& action);

    void setAction(const UnicodeString& value);
    const UnicodeString& getAction() const;

    void setParameter(const UnicodeString& value, unsigned int index = 0);
    const UnicodeString& getParameter(unsigned int index = 0) const;
    int getParameterInt(unsigned int index = 0) const;
    const UnicodeString* getParameterPtr() const;

    virtual void onClicked(BaseButton* self);

private:
    UnicodeString action_;
    UnicodeString parameter_[MAX_PARAMETER_COUNT];
};

}
}
}

#endif
