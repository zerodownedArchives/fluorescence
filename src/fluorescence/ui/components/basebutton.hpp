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


#ifndef FLUO_UI_COMPONENTS_BASEBUTTON_HPP
#define FLUO_UI_COMPONENTS_BASEBUTTON_HPP

#include <misc/string.hpp>

namespace fluo {
namespace ui {

class GumpMenu;

namespace components {

class BaseButton {
public:
    static const unsigned int MAX_PARAMETER_COUNT = 5;
    enum ButtonType {
        TYPE_UNDEFINED = 0,
        TYPE_PAGE = 1,
        TYPE_SERVER = 2,
        TYPE_LOCAL = 3,
    };
    
    void setServerButton(unsigned int buttonId);
    void setPageButton(unsigned int pageId);
    void setLocalButton(const UnicodeString& action);

    void handleClick();
    
    const UnicodeString& getAction() const;
    void setParameter(const UnicodeString& value, unsigned int index = 0);
    const UnicodeString* getParameterPtr() const;
    
    virtual GumpMenu* getTopLevelMenu() = 0;
    
protected:
    BaseButton();
    BaseButton(const BaseButton& other) { };
    BaseButton& operator=(const BaseButton& other) { return *this; };

private:
    unsigned int buttonType_;
    
    // for server button
    unsigned int buttonId_;
    
    // for page button
    unsigned int pageId_;
    
    // for local button
    UnicodeString action_;
    UnicodeString parameter_[MAX_PARAMETER_COUNT];
    
    void onClickPage();
    void onClickLocal();
    void onClickServer();
};

}
}
}

#endif

