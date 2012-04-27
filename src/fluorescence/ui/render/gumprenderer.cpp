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



#include "gumprenderer.hpp"

#include <ClanLib/Display/Render/program_object.h>

#include <ui/manager.hpp>
#include <ui/render/renderqueue.hpp>
#include <ui/render/shadermanager.hpp>
#include <ui/texture.hpp>
#include <ui/components/gumpview.hpp>

#include <misc/log.hpp>

#include <world/ingameobject.hpp>

#include <data/manager.hpp>
#include <data/huesloader.hpp>

namespace fluo {
namespace ui {

GumpRenderer::GumpRenderer(boost::shared_ptr<RenderQueue> renderQueue, components::GumpView* gumpView) : 
            gumpView_(gumpView), renderQueue_(renderQueue) {
}

GumpRenderer::~GumpRenderer() {
    renderQueue_->clear();
}


void GumpRenderer::checkTextureSize(CL_GraphicContext& gc) {
    if (texture_.is_null() || texture_.get_size() != gumpView_->get_size()) {
        texture_ = ui::Manager::getSingleton()->providerRenderBufferTexture(gumpView_->get_size());
        
        frameBuffer_ = CL_FrameBuffer(gc);
        frameBuffer_.attach_color_buffer(0, texture_);
    }
}

CL_Texture GumpRenderer::getTexture(CL_GraphicContext& gc) {
    if (renderQueue_->requireGumpRepaint() || texture_.is_null()) {
        checkTextureSize(gc);
        CL_FrameBuffer origBuffer = gc.get_write_frame_buffer();
        
        if (origBuffer.is_null()) {
            LOG_DEBUG << "oimg" << std::endl;
        }

        gc.set_frame_buffer(frameBuffer_);

        render(gc);

        gc.set_frame_buffer(origBuffer);
    }

    return texture_;
}


void GumpRenderer::render(CL_GraphicContext& gc) {
    renderQueue_->preRender();

    gc.clear(CL_Colorf(0.f, 0.f, 0.f, 0.f));

    boost::shared_ptr<CL_ProgramObject> shader = ui::Manager::getShaderManager()->getGumpShader();
    gc.set_program_object(*shader, cl_program_matrix_modelview_projection);

    CL_Texture huesTexture = data::Manager::getHuesLoader()->getHuesTexture();
    gc.set_texture(0, huesTexture);
    // set texture unit 1 active to avoid overriding the hue texture with newly loaded object textures
    gc.set_texture(1, huesTexture);
    
    shader->set_uniform1i("HueTexture", 0);
    shader->set_uniform1i("ObjectTexture", 1);

    RenderQueue::const_iterator igIter = renderQueue_->begin();
    RenderQueue::const_iterator igEnd = renderQueue_->end();

    CL_Vec2f vertexCoords[6];

    bool renderingComplete = true;

    for (; igIter != igEnd; ++igIter) {
        boost::shared_ptr<world::IngameObject> curObj = *igIter;

        // just mobiles and items in a gump
        if (!curObj->isMobile() && ! curObj->isDynamicItem()) {
            continue;
        }

        // check if texture is ready to be drawn
        ui::Texture* tex = curObj->getGumpTexture().get();

        if (!tex) {
            continue;
        }

        if (!tex->isReadComplete()) {
            renderingComplete = false;
            continue;
        }
        
        CL_Rectf texCoordHelper = tex->getNormalizedTextureCoords();

        CL_Vec2f texCoords[6] = {
            CL_Vec2f(texCoordHelper.left, texCoordHelper.top),
            CL_Vec2f(texCoordHelper.right, texCoordHelper.top),
            CL_Vec2f(texCoordHelper.left, texCoordHelper.bottom),
            CL_Vec2f(texCoordHelper.right, texCoordHelper.top),
            CL_Vec2f(texCoordHelper.left, texCoordHelper.bottom),
            CL_Vec2f(texCoordHelper.right, texCoordHelper.bottom)
        };

        CL_Rectf rect(0, 0, tex->getWidth(), tex->getHeight());

        vertexCoords[0] = CL_Vec2f(rect.left, rect.top);
        vertexCoords[1] = CL_Vec2f(rect.right, rect.top);
        vertexCoords[2] = CL_Vec2f(rect.left, rect.bottom);
        vertexCoords[3] = CL_Vec2f(rect.right, rect.top);
        vertexCoords[4] = CL_Vec2f(rect.left, rect.bottom);
        vertexCoords[5] = CL_Vec2f(rect.right, rect.bottom);

        CL_PrimitivesArray primarray(gc);
        primarray.set_attributes(0, vertexCoords);
        primarray.set_attributes(1, texCoords);

        primarray.set_attribute(2, curObj->getHueInfo());

        gc.set_texture(1, tex->getTexture());
        gc.draw_primitives(cl_triangles, 6, primarray);
    }

    gc.reset_textures();
    gc.reset_program_object();

    renderQueue_->postRender(renderingComplete);
    
    if (!renderingComplete) {
        gumpView_->request_repaint();
    }
}

boost::shared_ptr<RenderQueue> GumpRenderer::getRenderQueue() const {
    return renderQueue_;
}

}
}
