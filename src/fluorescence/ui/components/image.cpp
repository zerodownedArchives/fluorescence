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
#include <ClanLib/Display/Render/program_object.h>

#include <ui/manager.hpp>
#include <ui/render/shadermanager.hpp>
#include <ui/texture.hpp>
#include <data/manager.hpp>
#include <data/huesloader.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace components {

Image::Image(CL_GUIComponent* parent) : CL_GUIComponent(parent), autoResize_(false), hueInfo_(0, 0, 1), colorRgba_(CL_Colorf::white), requireInitialRepaint_(true) {
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
            if (hueInfo_[1u] == 0) {
                CL_Draw::texture(gc, *(texture_->getTexture()), CL_Quadf(CL_Rectf(0, 0, get_width(), get_height())), colorRgba_);
            } else {
                renderShader(gc, clipRect);
                
                if (requireInitialRepaint_) {
                    requireInitialRepaint_ = false;
                    request_repaint();
                }
            }
            
        }
    }
}

void Image::setColorRGBA(const CL_Colorf& color) {
    colorRgba_ = color;
}

void Image::setHue(unsigned int hue) {
    hueInfo_[1u] = hue;
}

void Image::setAlpha(float alpha) {
    hueInfo_[2u] = alpha;
    colorRgba_.a = alpha;
}

void Image::renderShader(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    //gc.clear(CL_Colorf(0.f, 0.f, 0.f, 0.f));

    boost::shared_ptr<CL_ProgramObject> shader = ui::Manager::getShaderManager()->getGumpShader();
    gc.set_program_object(*shader, cl_program_matrix_modelview_projection);

    CL_Rectf texture_unit1_coords(0.0f, 0.0f, 1.0f, 1.0f);

    CL_Vec2f tex1_coords[6] = {
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom)
    };


    gc.set_texture(0, *(data::Manager::getSingleton()->getHuesLoader()->getHuesTexture()->getTexture()));
    shader->set_uniform1i("HueTexture", 0);
    shader->set_uniform1i("ObjectTexture", 1);

    CL_Vec2f vertexCoords[6];

    CL_Rectf rect(0, 0, get_width(), get_height());

    vertexCoords[0] = CL_Vec2f(rect.left, rect.top);
    vertexCoords[1] = CL_Vec2f(rect.right, rect.top);
    vertexCoords[2] = CL_Vec2f(rect.left, rect.bottom);
    vertexCoords[3] = CL_Vec2f(rect.right, rect.top);
    vertexCoords[4] = CL_Vec2f(rect.left, rect.bottom);
    vertexCoords[5] = CL_Vec2f(rect.right, rect.bottom);

    CL_PrimitivesArray primarray(gc);
    primarray.set_attributes(0, vertexCoords);
    primarray.set_attributes(1, tex1_coords);

    primarray.set_attribute(2, hueInfo_);

    gc.set_texture(1, *texture_->getTexture());
    gc.draw_primitives(cl_triangles, 6, primarray);

    gc.reset_textures();
    gc.reset_program_object();
}

}
}
}

