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


#ifndef FLUO_UI_COMPONENTS_IMAGE_HPP
#define FLUO_UI_COMPONENTS_IMAGE_HPP

#include <ClanLib/GUI/gui_component.h>
#include <ClanLib/Display/Render/graphic_context.h>
#include <boost/shared_ptr.hpp>

namespace fluo {
namespace ui {

class Texture;
class GumpFactory;
    
namespace components {

class Image : public CL_GUIComponent {

friend class ui::GumpFactory;

public:
    Image(CL_GUIComponent* parent);

    void setTexture(boost::shared_ptr<ui::Texture> tex);
    void render(CL_GraphicContext& gc, const CL_Rect& clipRect);
    
private:
    boost::shared_ptr<ui::Texture> texture_;
    
    bool autoResize_;
};

}
}
}

#endif

