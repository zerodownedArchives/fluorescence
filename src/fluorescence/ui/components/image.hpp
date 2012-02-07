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
class XmlParser;
    
namespace components {

class Image : public CL_GUIComponent {

friend class ui::XmlParser;

public:
    Image(CL_GUIComponent* parent);

    void setTexture(boost::shared_ptr<ui::Texture> tex);
    void render(CL_GraphicContext& gc, const CL_Rect& clipRect);
    
    void setColorRGBA(const CL_Colorf& color);
    void setHue(unsigned int hue);
    void setAlpha(float alpha);
    
    void setTiled(bool tiled);
    void setStretchTexture(bool stretch);
    void setAutoResize(bool value);
    
protected:
    void setHueInfo(const CL_Vec3f& hueInfo);
    
private:
    boost::shared_ptr<ui::Texture> texture_;
    
    bool autoResize_;
    
    CL_Vec3f hueInfo_;
    CL_Colorf colorRgba_;
    
    void renderShader(CL_GraphicContext& gc, const CL_Rect& clipRect);
    
    // due to some strange clanlib behaviour. when opening a paperdoll for the first time, the hue texture seems
    // not to be loaded correctly, so the gump is mostly black (with outlines). When rendering it again, all seems
    // fine. this flag indicates wheter the renderer has to re-render the image because of this condition
    bool requireInitialRepaint_;
    
    bool stretchTexture_;
    bool tiled_;
    CL_Rectf textureRect_;
    
    void calculateTextureCoordinates();
};

}
}
}

#endif

