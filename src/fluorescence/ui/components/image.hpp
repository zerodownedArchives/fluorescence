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

#include <ClanLib/Display/Render/graphic_context.h>
#include <ClanLib/Display/Render/texture.h>
#include <boost/shared_ptr.hpp>

#include <ui/gumpcomponent.hpp>

namespace fluo {
namespace ui {

class Texture;
class XmlParser;

namespace components {

class Image : public GumpComponent {

friend class ui::XmlParser;

public:
    Image(CL_GUIComponent* parent);

    boost::shared_ptr<ui::Texture> getTexture() const;
    void setTexture(boost::shared_ptr<ui::Texture> tex);

    void render(CL_GraphicContext& gc, const CL_Rect& clipRect);

    void setTiled(bool value);
    virtual void setAutoResize(bool value);

    virtual bool has_pixel(const CL_Point& p) const;

private:
    boost::shared_ptr<ui::Texture> texture_;

    bool autoResize_;

    void renderShader(CL_GraphicContext& gc, const CL_Rect& clipRect);

    bool tiled_;

    CL_Texture tileableTexture_;
};

}
}
}

#endif

