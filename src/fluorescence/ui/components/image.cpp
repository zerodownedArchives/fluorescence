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

Image::Image(CL_GUIComponent* parent) : GumpComponent(parent),
        autoResize_(false), tiled_(false) {
    func_render().set(this, &Image::render);

    set_type_name("image");
}

void Image::setTexture(boost::shared_ptr<ui::Texture> tex) {
    texture_ = tex;
    tileableTexture_ = CL_Texture();
    request_repaint();
}

void Image::render(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    CL_Rectf geom = get_geometry();
    if (!texture_ || !texture_->isReadComplete()) {
        ui::Manager::getSingleton()->queueComponentRepaint(this);
    } else if (autoResize_ && (geom.get_width() != texture_->getWidth() || geom.get_height() != texture_->getHeight())) {
        geom.set_width(texture_->getWidth());
        geom.set_height(texture_->getHeight());

        ui::Manager::getSingleton()->queueComponentResize(this, geom);
        // repainted automatically after resize
    } else if (!tiled_) {
        if (useRgba_) {
            CL_Draw::texture(gc, texture_->getTexture(), CL_Quadf(CL_Rectf(0, 0, get_width(), get_height())), rgba_, texture_->getNormalizedTextureCoords());
        } else {
            renderShader(gc, clipRect);
        }
    } else {
        if (tileableTexture_.is_null()) {
            // create CL_Texture from ui::Texture. can be very expensive, unfortunately.
            // maybe find a better solution (tile and crop textures manually here instead of relying on tex coords?)
            tileableTexture_ = texture_->extractSingleTexture();
            tileableTexture_.set_wrap_mode(cl_wrap_repeat , cl_wrap_repeat );
        }

        if (useRgba_) {
            CL_Draw::texture(gc, tileableTexture_,
                    CL_Quadf(CL_Rectf(0, 0, get_width(), get_height())),
                    rgba_,
                    CL_Rectf(0.0f, 0.0f, get_width() / texture_->getWidth(), get_height() / texture_->getHeight()));
        } else {
            renderShader(gc, clipRect);
        }
    }
}

void Image::setTiled(bool value) {
    tiled_ = value;
}

void Image::setAutoResize(bool value) {
    autoResize_ = value;
    if (autoResize_) {
        tiled_ = false;
    }
}

void Image::renderShader(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    boost::shared_ptr<CL_ProgramObject> shader = ui::Manager::getShaderManager()->getGumpShader();
    gc.set_program_object(*shader, cl_program_matrix_modelview_projection);

    CL_Texture huesTexture = data::Manager::getHuesLoader()->getHuesTexture();
    gc.set_texture(0, huesTexture);
    // set texture unit 1 active to avoid overriding the hue texture with newly loaded object textures
    gc.set_texture(1, huesTexture);

    shader->set_uniform1i("HueTexture", 0);
    shader->set_uniform1i("ObjectTexture", 1);

    CL_Rectf textureRect;

    if (tiled_) {
        gc.set_texture(1, tileableTexture_);
        textureRect = CL_Rectf(0.0f, 0.0f, get_width() / texture_->getWidth(), get_height() / texture_->getHeight());
    } else {
        gc.set_texture(1, texture_->getTexture());
        textureRect = texture_->getNormalizedTextureCoords();
    }

    CL_Vec2f tex1_coords[6] = {
        CL_Vec2f(textureRect.left, textureRect.top),
        CL_Vec2f(textureRect.right, textureRect.top),
        CL_Vec2f(textureRect.left, textureRect.bottom),
        CL_Vec2f(textureRect.right, textureRect.top),
        CL_Vec2f(textureRect.left, textureRect.bottom),
        CL_Vec2f(textureRect.right, textureRect.bottom)
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

    CL_Vec3f hueVec = getHueVector();
    primarray.set_attribute(2, hueVec);

    gc.draw_primitives(cl_triangles, 6, primarray);

    gc.reset_textures();
    gc.reset_program_object();
}

bool Image::has_pixel(const CL_Point& p) const {
    if (!texture_) {
        return false;
    }

    int px = p.x;
    int py = p.y;

    if (tiled_) {
        if (tileableTexture_.is_null()) {
            return false;
        } else {
            px %= tileableTexture_.get_width();
            py %= tileableTexture_.get_height();
        }
    } else {
        float stretchHori = (float)get_width() / texture_->getWidth();
        float stretchVert = (float)get_height() / texture_->getHeight();
        px /= stretchHori;
        py /= stretchVert;
    }

    return texture_->hasPixel(px, py);
}

boost::shared_ptr<ui::Texture> Image::getTexture() const {
    return texture_;
}

}
}
}

