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


#ifndef FLUO_UI_GUMPACTIONS_HPP
#define FLUO_UI_GUMPACTIONS_HPP

#include <ui/gumpmenu.hpp>

#include <boost/function.hpp>
#include <map>

namespace fluo {
namespace ui {

namespace components {
class Button;
}

typedef boost::function<bool (GumpMenu*, const UnicodeString&, unsigned int, const UnicodeString*)> GumpActionCallback;

class GumpActions {
public:
    static void buildBasicActionTable();
    static void buildFullActionTable();

    static void doAction(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);
    static void doAction(components::Button* button);

private:

    struct GumpAction {
        GumpAction();

        GumpAction(bool closeGumpMenu, GumpActionCallback callback);

        bool closeGumpMenu_;
        GumpActionCallback callback_;
    };

    static std::map<UnicodeString, GumpAction> actionTable_;

    static bool closeHelper(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);

    static bool createDummyCharacter(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);

    static bool yesNoGump(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);

};

}
}

#endif
