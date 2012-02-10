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

Background::Background(CL_GUIComponent* parent) : CL_GUIComponent(parent), hueInfo_(0, 0, 1), colorRgba_(CL_Colorf::white), requireInitialRepaint_(true) {
    func_render().set(this, &Background::render);
}

void Background::setBaseId(unsigned int id) {
    for (unsigned int i = 0; i < 9; ++i) {
        textures_[i] = data::Manager::getGumpArtLoader()->getTexture(i + id);
    }
}

void Background::render(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    for (unsigned int i = 0; i < 9; ++i) {
        if (!textures_[i] || !textures_[i]->isReadComplete()) {
            request_repaint();
            return; // don't draw until everything is ready
        }
    }
    
    if (requireInitialRepaint_) {
        // drawing this the first time - calculate texture coordinates
        if (hueInfo_[1u] == 0) {
            calculateQuadCoordinates();
        } else {
            request_repaint();
            calculateVertexCoordinates();
        }
        requireInitialRepaint_ = false;
    }
    
    if (hueInfo_[1u] == 0) {
        for (unsigned int i = 0; i < 9; ++i) {
            CL_Draw::texture(gc, *(textures_[i]->getTexture()), quadCoords_[i], colorRgba_, quadTextureCoords_[i]);
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
    textures_[1]->getTexture()->set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);
    
    quadCoords_[2] = CL_Rectf(width - texWidths[2], 0, CL_Sizef(texWidths[2], texHeights[2]));
    quadTextureCoords_[2] = defaultTexCoords;
    
    quadCoords_[3] = CL_Rectf(0, texHeights[0], CL_Sizef(texWidths[3], middleBarHeightLeft));
    quadTextureCoords_[3] = CL_Rectf(0.0f, 0.0f, 1.0, middleBarHeightLeft / texHeights[3]);
    textures_[3]->getTexture()->set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);
    
    quadCoords_[4] = CL_Rectf(texWidths[3], texHeights[1], CL_Sizef(centerPieceWidth, centerPieceHeight));
    quadTextureCoords_[4] = CL_Rectf(0.0f, 0.0f, centerPieceWidth / texWidths[4], centerPieceHeight / texHeights[4]);
    textures_[4]->getTexture()->set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);
    
    quadCoords_[5] = CL_Rectf(width - texWidths[5], texHeights[2], CL_Sizef(texWidths[5], middleBarHeightRight));
    quadTextureCoords_[5] = CL_Rectf(0.0f, 0.0f, 1.0f, middleBarHeightRight / texHeights[5]);
    textures_[5]->getTexture()->set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);
    
    quadCoords_[6] = CL_Rectf(0, height - texHeights[6], CL_Sizef(texWidths[6], texHeights[6]));
    quadTextureCoords_[6] = defaultTexCoords;
    
    quadCoords_[7] = CL_Rectf(texWidths[6], height - texHeights[7], CL_Sizef(centerBarWidthBottom, texHeights[7]));
    quadTextureCoords_[7] = CL_Rectf(0.0f, 0.0f, centerBarWidthBottom / texWidths[7], 1.0);;
    textures_[7]->getTexture()->set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);
    
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
    
    LOG_DEBUG << "center text coords: " << centerPieceTexCoords << std::endl;
    
    // top left corner
    setVertexCoordinates(0, CL_Rectf(0, 0, CL_Sizef(texWidths[0], texHeights[0])));
    setTextureCoordinates(0, defaultTexCoords);
    
    // top center bar
    setVertexCoordinates(1, CL_Rectf(texWidths[0], 0, CL_Sizef(centerBarWidth, texHeights[1])));
    setTextureCoordinates(1, centerBarTexCoords);
    textures_[1]->getTexture()->set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);
    
    // top right corner
    setVertexCoordinates(2, CL_Rectf(width - texWidths[2], 0, CL_Sizef(texWidths[2], texHeights[2])));
    setTextureCoordinates(2, defaultTexCoords);
    
    // middle left bar
    setVertexCoordinates(3, CL_Rectf(0, texHeights[0], CL_Sizef(texWidths[3], middleBarHeight)));
    setTextureCoordinates(3, middleBarTexCoords);
    textures_[3]->getTexture()->set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);
    
    // center piece
    setVertexCoordinates(4, CL_Rectf(texWidths[0], texHeights[0], CL_Sizef(centerBarWidth, middleBarHeight)));
    setTextureCoordinates(4, centerPieceTexCoords);
    textures_[4]->getTexture()->set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);
    
    // middle right bar
    setVertexCoordinates(5, CL_Rectf(width - texWidths[2], texHeights[0], CL_Sizef(texWidths[5], middleBarHeight)));
    setTextureCoordinates(5, middleBarTexCoords);
    textures_[5]->getTexture()->set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);
    
    // bottom left corner
    setVertexCoordinates(6, CL_Rectf(0, height - texHeights[6], CL_Sizef(texWidths[6], texHeights[6])));
    setTextureCoordinates(6, defaultTexCoords);
    
    // bottom center bar
    setVertexCoordinates(7, CL_Rectf(texWidths[0], height - texHeights[6], CL_Sizef(centerBarWidth, texHeights[7])));
    setTextureCoordinates(7, centerBarTexCoords);
    textures_[7]->getTexture()->set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);
    
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

void Background::setColorRGBA(const CL_Colorf& color) {
    colorRgba_ = color;
}

void Background::setHue(unsigned int hue) {
    hueInfo_[1u] = hue;
}

void Background::setAlpha(float alpha) {
    hueInfo_[2u] = alpha;
    colorRgba_.a = alpha;
}

void Background::renderShader(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    //gc.clear(CL_Colorf(0.f, 0.f, 0.f, 0.f));

    boost::shared_ptr<CL_ProgramObject> shader = ui::Manager::getShaderManager()->getGumpShader();
    gc.set_program_object(*shader, cl_program_matrix_modelview_projection);

    gc.set_texture(0, *(data::Manager::getSingleton()->getHuesLoader()->getHuesTexture()->getTexture()));
    shader->set_uniform1i("HueTexture", 0);
    shader->set_uniform1i("ObjectTexture", 1);

    for (unsigned int i = 0; i < 9; ++i) {
        CL_PrimitivesArray primarray(gc);
        primarray.set_attributes(0, vertexCoords_[i]);
        primarray.set_attributes(1, textureCoords_[i]);

        primarray.set_attribute(2, hueInfo_);

        gc.set_texture(1, *textures_[i]->getTexture());
        gc.draw_primitives(cl_triangles, 6, primarray);
    }

    gc.reset_textures();
    gc.reset_program_object();
}

}
}
}

