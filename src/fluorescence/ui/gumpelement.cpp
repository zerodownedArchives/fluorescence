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



#include "gumpelement.hpp"

#include "gumpmenu.hpp"
#include "manager.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace ui {

GumpElement::GumpElement(CL_GUIComponent* parent) :
    CL_GUIComponent(parent) {
        
    func_pointer_enter().set(this, &GumpElement::onPointerEnter);
    func_pointer_exit().set(this, &GumpElement::onPointerExit);
}

bool GumpElement::onPointerEnter() {
    // LOG_DEBUG << "elem pointer enter" << std::endl;
    // TODO change cursor properties
    return false;
}

bool GumpElement::onPointerExit() {
    //LOG_DEBUG << "elem pointer exit" << std::endl;
    // TODO change cursor properties
    return false;
}

}
}
