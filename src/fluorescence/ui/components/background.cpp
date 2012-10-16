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

#include "background.hpp"

#include <ClanLib/Display/2D/draw.h>
#include <ClanLib/Core/Math/quad.h>
#include <ClanLib/Core/Math/rect.h>
#include <ClanLib/Display/Render/program_object.h>

#include <ui/manager.hpp>
#include <ui/render/shadermanager.hpp>
#include <ui/texture.hpp>
#include <data/manager.hpp>
#include <data/huesloader.hpp>
#include <data/gumpartloader.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace components {

Background::Background(CL_GUIComponent* parent) : GumpComponent(parent), baseId_(0) {
    func_render().set(this, &Background::render);

    set_type_name("background");
}

void Background::setBaseId(unsigned int id) {
    baseId_ = id;
    for (unsigned int i = 0; i < 9; ++i) {
        textures_[i] = data::Manager::getTexture(data::TextureSource::GUMPART, i + id);
    }
}

unsigned int Background::getBaseId() const {
    return baseId_;
}

void Background::render(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    for (unsigned int i = 0; i < 9; ++i) {
        if (!textures_[i] || !textures_[i]->isReadComplete()) {
            ui::Manager::getSingleton()->queueComponentRepaint(this);
            return; // don't draw until everything is ready
        }
    }

    if (extractedTextures_[0].is_null()) {
        // create CL_Texture from ui::Texture. can be very expensive, if the CL_Subtexture was already initialized.
        // However, this should usually not be the case, as the background texture are most often used as backgrounds (i.e. in this class)
        // maybe find a better solution (tile and crop textures manually here instead of relying on tex coords?)
        for (unsigned int i = 0; i < 9; ++i) {
            extractedTextures_[i] = textures_[i]->extractSingleTexture();
        }

        if (hue_ == 0) {
            calculateQuadCoordinates();
        } else {
            calculateVertexCoordinates();
        }
    }

    if (hue_ == 0) {
        for (unsigned int i = 0; i < 9; ++i) {
            CL_Draw::texture(gc, extractedTextures_[i], quadCoords_[i], rgba_, quadTextureCoords_[i]);
        }
    } else {
        renderShader(gc, clipRect);
    }
}

void Background::calculateQuadCoordinates() {
    float width = get_geometry().get_width();
    float height = get_geometry().get_height();
    CL_Rectf defaultTexCoords(0.0f, 0.0f, 1.0f, 1.0f);

    float texWidths[9];
    float texHeights[9];

    for (unsigned int i = 0; i < 9; ++i) {
        texWidths[i] = (std::min)(width, textures_[i]->getWidth());
        texHeights[i] = (std::min)(height, textures_[i]->getHeight());
    }

    float centerBarWidthTop = width - (texWidths[0] + texWidths[2]);
    float centerBarWidthBottom = width - (texWidths[6] + texWidths[8]);
    float middleBarHeightLeft = height - (texHeights[0] + texHeights[6]);
    float middleBarHeightRight = height - (texHeights[2] + texHeights[8]);

    float centerPieceWidth = width - (texWidths[3] + texWidths[5]);
    float centerPieceHeight = height - (texHeights[1] + texHeights[7]);

    quadCoords_[0] = CL_Rectf(0, 0, CL_Sizef(texWidths[0], texHeights[0]));
    quadTextureCoords_[0] = defaultTexCoords;

    quadCoords_[1] = CL_Rectf(texWidths[0], 0, CL_Sizef(centerBarWidthTop, texHeights[1]));
    quadTextureCoords_[1] = CL_Rectf(0.0f, 0.0f, centerBarWidthTop / texWidths[1], 1.0);;
    extractedTextures_[1].set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);

    quadCoords_[2] = CL_Rectf(width - texWidths[2], 0, CL_Sizef(texWidths[2], texHeights[2]));
    quadTextureCoords_[2] = defaultTexCoords;

    quadCoords_[3] = CL_Rectf(0, texHeights[0], CL_Sizef(texWidths[3], middleBarHeightLeft));
    quadTextureCoords_[3] = CL_Rectf(0.0f, 0.0f, 1.0, middleBarHeightLeft / texHeights[3]);
    extractedTextures_[3].set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);

    quadCoords_[4] = CL_Rectf(texWidths[3], texHeights[1], CL_Sizef(centerPieceWidth, centerPieceHeight));
    quadTextureCoords_[4] = CL_Rectf(0.0f, 0.0f, centerPieceWidth / texWidths[4], centerPieceHeight / texHeights[4]);
    extractedTextures_[4].set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);

    quadCoords_[5] = CL_Rectf(width - texWidths[5], texHeights[2], CL_Sizef(texWidths[5], middleBarHeightRight));
    quadTextureCoords_[5] = CL_Rectf(0.0f, 0.0f, 1.0f, middleBarHeightRight / texHeights[5]);
    extractedTextures_[5].set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);

    quadCoords_[6] = CL_Rectf(0, height - texHeights[6], CL_Sizef(texWidths[6], texHeights[6]));
    quadTextureCoords_[6] = defaultTexCoords;

    quadCoords_[7] = CL_Rectf(texWidths[6], height - texHeights[7], CL_Sizef(centerBarWidthBottom, texHeights[7]));
    quadTextureCoords_[7] = CL_Rectf(0.0f, 0.0f, centerBarWidthBottom / texWidths[7], 1.0);;
    extractedTextures_[7].set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);

    quadCoords_[8] = CL_Rectf(width - texWidths[8], height - texHeights[8], CL_Sizef(texWidths[8], texHeights[8]));
    quadTextureCoords_[8] = defaultTexCoords;
}

void Background::calculateVertexCoordinates() {
    float width = get_geometry().get_width();
    float height = get_geometry().get_height();
    CL_Rectf defaultTexCoords(0.0f, 0.0f, 1.0f, 1.0f);

    float texWidths[9];
    float texHeights[9];

    for (unsigned int i = 0; i < 9; ++i) {
        texWidths[i] = textures_[i]->getWidth();
        texHeights[i] = textures_[i]->getHeight();
    }

    float centerBarWidth = width - (texWidths[0] + texWidths[2]);
    CL_Rectf centerBarTexCoords(0.0f, 0.0f, centerBarWidth / texWidths[1], 1.0);

    float middleBarHeight = height - (texHeights[0] + texHeights[6]);
    CL_Rectf middleBarTexCoords(0.0f, 0.0f, 1.0, middleBarHeight / texHeights[3]);

    CL_Rectf centerPieceTexCoords(0.0f, 0.0f, centerBarWidth / texWidths[4], middleBarHeight / texHeights[4]);

    // top left corner
    setVertexCoordinates(0, CL_Rectf(0, 0, CL_Sizef(texWidths[0], texHeights[0])));
    setTextureCoordinates(0, defaultTexCoords);

    // top center bar
    setVertexCoordinates(1, CL_Rectf(texWidths[0], 0, CL_Sizef(centerBarWidth, texHeights[1])));
    setTextureCoordinates(1, centerBarTexCoords);
    extractedTextures_[1].set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);

    // top right corner
    setVertexCoordinates(2, CL_Rectf(width - texWidths[2], 0, CL_Sizef(texWidths[2], texHeights[2])));
    setTextureCoordinates(2, defaultTexCoords);

    // middle left bar
    setVertexCoordinates(3, CL_Rectf(0, texHeights[0], CL_Sizef(texWidths[3], middleBarHeight)));
    setTextureCoordinates(3, middleBarTexCoords);
    extractedTextures_[3].set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);

    // center piece
    setVertexCoordinates(4, CL_Rectf(texWidths[0], texHeights[0], CL_Sizef(centerBarWidth, middleBarHeight)));
    setTextureCoordinates(4, centerPieceTexCoords);
    extractedTextures_[4].set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);

    // middle right bar
    setVertexCoordinates(5, CL_Rectf(width - texWidths[2], texHeights[0], CL_Sizef(texWidths[5], middleBarHeight)));
    setTextureCoordinates(5, middleBarTexCoords);
    extractedTextures_[5].set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);

    // bottom left corner
    setVertexCoordinates(6, CL_Rectf(0, height - texHeights[6], CL_Sizef(texWidths[6], texHeights[6])));
    setTextureCoordinates(6, defaultTexCoords);

    // bottom center bar
    setVertexCoordinates(7, CL_Rectf(texWidths[0], height - texHeights[6], CL_Sizef(centerBarWidth, texHeights[7])));
    setTextureCoordinates(7, centerBarTexCoords);
    extractedTextures_[7].set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);

    // bottom right corner
    setVertexCoordinates(8, CL_Rectf(width - texWidths[8], height - texHeights[8], CL_Sizef(texWidths[8], texHeights[8])));
    setTextureCoordinates(8, defaultTexCoords);
}

void Background::setVertexCoordinates(unsigned int index, const CL_Rectf& rect) {
    vertexCoords_[index][0] = CL_Vec2f(rect.left, rect.top);
    vertexCoords_[index][1] = CL_Vec2f(rect.right, rect.top);
    vertexCoords_[index][2] = CL_Vec2f(rect.left, rect.bottom);
    vertexCoords_[index][3] = CL_Vec2f(rect.right, rect.top);
    vertexCoords_[index][4] = CL_Vec2f(rect.left, rect.bottom);
    vertexCoords_[index][5] = CL_Vec2f(rect.right, rect.bottom);
}

void Background::setTextureCoordinates(unsigned int index, const CL_Rectf& rect) {
    textureCoords_[index][0] = CL_Vec2f(rect.left, rect.top);
    textureCoords_[index][1] = CL_Vec2f(rect.right, rect.top);
    textureCoords_[index][2] = CL_Vec2f(rect.left, rect.bottom);
    textureCoords_[index][3] = CL_Vec2f(rect.right, rect.top);
    textureCoords_[index][4] = CL_Vec2f(rect.left, rect.bottom);
    textureCoords_[index][5] = CL_Vec2f(rect.right, rect.bottom);
}

void Background::renderShader(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    //gc.clear(CL_Colorf(0.f, 0.f, 0.f, 0.f));

    boost::shared_ptr<CL_ProgramObject> shader = ui::Manager::getShaderManager()->getGumpShader();
    gc.set_program_object(*shader, cl_program_matrix_modelview_projection);

    CL_Texture huesTexture = data::Manager::getHuesLoader()->getHuesTexture();
    gc.set_texture(0, huesTexture);
    // set texture unit 1 active to avoid overriding the hue texture with newly loaded object textures
    gc.set_texture(1, huesTexture);

    shader->set_uniform1i("HueTexture", 0);
    shader->set_uniform1i("ObjectTexture", 1);

    for (unsigned int i = 0; i < 9; ++i) {
        CL_PrimitivesArray primarray(gc);
        primarray.set_attributes(0, vertexCoords_[i]);
        primarray.set_attributes(1, textureCoords_[i]);

        primarray.set_attribute(2, getHueVector());

        gc.set_texture(1, extractedTextures_[i]);
        gc.draw_primitives(cl_triangles, 6, primarray);
    }

    gc.reset_textures();
    gc.reset_program_object();
}

void Background::setAutoResize(bool value) {
    if (value) {
        LOG_WARN << "Auto resizing is not supported on background components" << std::endl;
    }
}

}
}
}

