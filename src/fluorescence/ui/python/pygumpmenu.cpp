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

#include "pygumpmenu.hpp"

#include <ui/gumpmenu.hpp>

namespace fluo {
namespace ui {
namespace python {

GumpMenu* PyGumpMenu::create(int x, int y) {
    return createBackground(x, y, false);
}

GumpMenu* PyGumpMenu::createBackground(int x, int y, bool inBackground) {
    CL_GUITopLevelDescription desc(CL_Rect(x, y, CL_Size(1, 1)), false);
    desc.set_decorations(false);
    desc.set_in_background(inBackground);

    return new GumpMenu(desc);
}

}
}
}

