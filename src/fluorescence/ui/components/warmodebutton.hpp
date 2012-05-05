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


#ifndef FLUO_UI_COMPONENTS_WARMODEBUTTON_HPP
#define FLUO_UI_COMPONENTS_WARMODEBUTTON_HPP

#include <boost/shared_ptr.hpp>
#include <ClanLib/Display/Window/input_event.h>

#include "multitextureimage.hpp"
#include "basebutton.hpp"

namespace fluo {
namespace ui {
namespace components {

class WarModeButton : public MultiTextureImage<6>, public BaseButton {

friend class ui::XmlParser;

public:
    enum TextureIndex {
        TEX_INDEX_UP = 0,
        TEX_INDEX_MOUSEOVER = 1,
        TEX_INDEX_DOWN = 2,
        
        WARMODE_TEX_INDEX_UP = 3,
        WARMODE_TEX_INDEX_MOUSEOVER = 4,
        WARMODE_TEX_INDEX_DOWN = 5,
    };

    WarModeButton(CL_GUIComponent* parent);
    
    virtual GumpMenu* getTopLevelMenu();
    
    void updateTexture();
    
    void setWarMode(bool warmode);
    
private:
    bool onInputPressed(const CL_InputEvent & e);
    bool onInputReleased(const CL_InputEvent & e);
    
    bool onPointerEnter();
    bool onPointerExit();
    
    bool mouseOver_;
    bool mouseDown_;
    
    unsigned int calcTextureId() const;
    
    bool warmode_;
};

}
}
}

#endif
