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
#include <ClanLib/Display/Render/texture.h>
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
    
    virtual void setColorRGBA(const CL_Colorf& color);
    virtual void setHue(unsigned int hue);
    virtual void setAlpha(float alpha);
    
    void setTiled(bool value);
    void setAutoResize(bool value);
    
    bool has_pixel(const CL_Point& p) const;
    
protected:
    void setHueInfo(const CL_Vec3f& hueInfo);
    
private:
    boost::shared_ptr<ui::Texture> texture_;
    
    bool autoResize_;
    
    CL_Vec3f hueInfo_;
    CL_Colorf colorRgba_;
    
    void renderShader(CL_GraphicContext& gc, const CL_Rect& clipRect);
    
    bool tiled_;
    
    CL_Texture tileableTexture_;
};

}
}
}

#endif

