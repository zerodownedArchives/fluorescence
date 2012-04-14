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

Image::Image(CL_GUIComponent* parent) : CL_GUIComponent(parent), 
        autoResize_(false), hueInfo_(0, 0, 1), colorRgba_(CL_Colorf::white), stretchTexture_(true), tiled_(false) {
    func_render().set(this, &Image::render);
}

void Image::setTexture(boost::shared_ptr<ui::Texture> tex) {
    texture_ = tex;
    request_repaint();
    calculateTextureCoordinates();
    stretchableTexture_ = CL_Texture();
}

void Image::render(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    CL_Rectf geom = get_geometry();
    if (!texture_ || !texture_->isReadComplete()) {
        request_repaint();
    } else if (autoResize_ && (geom.get_width() != texture_->getWidth() || geom.get_height() != texture_->getHeight())) {
        geom.set_width(texture_->getWidth());
        geom.set_height(texture_->getHeight());
        set_geometry(geom);
        calculateTextureCoordinates();
        request_repaint();
    } else if (!tiled_ && stretchTexture_) {
        if (hueInfo_[1u] == 0) {
            CL_Draw::texture(gc, texture_->getTexture(), CL_Quadf(CL_Rectf(0, 0, get_width(), get_height())), colorRgba_, texture_->getNormalizedTextureCoords());
        } else {
            renderShader(gc, clipRect);
        }
    } else {
        if (stretchableTexture_.is_null()) {
            // create CL_Texture from ui::Texture. can be very expensive, unfortunately.
            // maybe find a better solution (tile and crop textures manually here instead of relying on tex coords?)
            stretchableTexture_ = texture_->extractSingleTexture();
            
            if (tiled_) {
                stretchableTexture_.set_wrap_mode(cl_wrap_repeat , cl_wrap_repeat );
            } else {
                stretchableTexture_.set_wrap_mode(cl_wrap_clamp_to_edge , cl_wrap_clamp_to_edge);
            }
        }
        
        if (hueInfo_[1u] == 0) {
            CL_Draw::texture(gc, stretchableTexture_, CL_Quadf(CL_Rectf(0, 0, get_width(), get_height())), colorRgba_, textureRect_);
        } else {
            renderShader(gc, clipRect);
        }
    }
}

void Image::calculateTextureCoordinates() {
    if (tiled_) {
        textureRect_ = CL_Rectf(0.0f, 0.0f, get_width() / texture_->getWidth(), get_height() / texture_->getHeight());
    } else if (stretchTexture_) {
        textureRect_ = CL_Rectf(0, 0, 1, 1);
    } else {
        textureRect_ = CL_Rectf(0.0f, 0.0f, get_width() / texture_->getWidth(), get_height() / texture_->getHeight());
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

void Image::setTiled(bool tiled) {
    tiled_ = tiled;
}

void Image::setStretchTexture(bool stretch) {
    stretchTexture_ = stretch;
}

void Image::setHueInfo(const CL_Vec3f& info) {
    hueInfo_ = info;
}

void Image::setAutoResize(bool value) {
    autoResize_ = value;
}

void Image::renderShader(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    //gc.clear(CL_Colorf(0.f, 0.f, 0.f, 0.f));

    boost::shared_ptr<CL_ProgramObject> shader = ui::Manager::getShaderManager()->getGumpShader();
    gc.set_program_object(*shader, cl_program_matrix_modelview_projection);


    gc.set_texture(0, data::Manager::getHuesLoader()->getHuesTexture());
    shader->set_uniform1i("HueTexture", 0);
    shader->set_uniform1i("ObjectTexture", 1);
    
    if (!tiled_ && stretchTexture_) {
        gc.set_texture(1, texture_->getTexture());
        textureRect_ = texture_->getNormalizedTextureCoords();
    } else {
        gc.set_texture(1, stretchableTexture_);
    }
    
	CL_Vec2f tex1_coords[6] = {
        CL_Vec2f(textureRect_.left, textureRect_.top),
        CL_Vec2f(textureRect_.right, textureRect_.top),
        CL_Vec2f(textureRect_.left, textureRect_.bottom),
        CL_Vec2f(textureRect_.right, textureRect_.top),
        CL_Vec2f(textureRect_.left, textureRect_.bottom),
        CL_Vec2f(textureRect_.right, textureRect_.bottom)
	};

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

    gc.draw_primitives(cl_triangles, 6, primarray);

    gc.reset_textures();
    gc.reset_program_object();
}

}
}
}

