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

#include "image.hpp"

#include <ClanLib/Display/2D/draw.h>
#include <ClanLib/Core/Math/quad.h>
#include <ClanLib/Core/Math/rect.h>

#include <ui/texture.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace components {

Image::Image(CL_GUIComponent* parent) : CL_GUIComponent(parent), autoResize_(false) {
    func_render().set(this, &Image::render);
}

void Image::setTexture(boost::shared_ptr<ui::Texture> tex) {
    texture_ = tex;
}

void Image::render(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    if (!texture_ || !texture_->isReadComplete()) {
        request_repaint();
    } else {
        CL_Rectf geom = get_geometry();
        if (autoResize_ && (geom.get_width() != texture_->getWidth() || geom.get_height() != texture_->getHeight())) {
            geom.set_width(texture_->getWidth());
            geom.set_height(texture_->getHeight());
            set_geometry(geom);
            request_repaint();
        } else {
            // TODO: draw with shader (coloring)
            CL_Draw::texture(gc, *(texture_->getTexture()), CL_Quadf(CL_Rectf(0, 0, texture_->getWidth(), texture_->getHeight())));
        }
    }
}

}
}
}

