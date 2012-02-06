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


#ifndef FLUO_UI_COMPONENTS_UOCHECKBOX_HPP
#define FLUO_UI_COMPONENTS_UOCHECKBOX_HPP

#include <boost/shared_ptr.hpp>
#include <ClanLib/Display/Window/input_event.h>

#include "multitextureimage.hpp"
#include "basebutton.hpp"

namespace fluo {
namespace ui {
namespace components {

class UoCheckbox : public MultiTextureImage<4> {

friend class ui::XmlParser;

public:
    UoCheckbox(CL_GUIComponent* parent);
    
    void updateTexture();
    
    void setChecked(bool value);
    bool isChecked() const;
    
private:
    bool onInputPressed(const CL_InputEvent & e);
    
    bool onPointerEnter();
    bool onPointerExit();
    
    bool mouseOver_;
    bool checked_;
    
    unsigned int switchId_;
    
    unsigned int calcTextureId() const;
};

}
}
}

#endif
