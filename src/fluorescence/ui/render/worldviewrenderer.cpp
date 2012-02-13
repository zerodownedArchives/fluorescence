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



#include "worldviewrenderer.hpp"

#include <ClanLib/Display/Render/program_object.h>

#include <ui/manager.hpp>
#include <ui/render/renderqueue.hpp>
#include <ui/render/shadermanager.hpp>
#include <ui/texture.hpp>
#include <ui/fontengine.hpp>
#include <ui/components/worldview.hpp>

#include <misc/log.hpp>

#include <world/ingameobject.hpp>
#include <world/lightmanager.hpp>
#include <world/manager.hpp>
#include <world/particleeffect.hpp>
#include <world/sectormanager.hpp>
#include <world/sector.hpp>

#include <data/manager.hpp>
#include <data/huesloader.hpp>



#include <ui/particles/xmlloader.hpp>

namespace fluo {
namespace ui {

WorldViewRenderer::WorldViewRenderer(boost::shared_ptr<RenderQueue> renderQueue, components::WorldView* worldView) : IngameObjectRenderer(IngameObjectRenderer::TYPE_WORLD),
        worldView_(worldView), renderQueue_(renderQueue) {
}

WorldViewRenderer::~WorldViewRenderer() {
    renderQueue_->clear();
}

void WorldViewRenderer::checkTextureSize() {
    if (!texture_ || texture_->getWidth() != worldView_->getWidth() || texture_->getHeight() != worldView_->getHeight()) {
        texture_.reset(new ui::Texture(false));
        texture_->initPixelBuffer(worldView_->getWidth(), worldView_->getHeight());
        texture_->setReadComplete();

        CL_GraphicContext gc = ui::Manager::getGraphicContext();

        depthTexture_ = CL_Texture(gc, gc.get_size(), cl_depth_component);
    }
}

boost::shared_ptr<Texture> WorldViewRenderer::getTexture(CL_GraphicContext& gc) {
    if (true || renderQueue_->requireWorldRepaint() || !texture_ || requireInitialRepaint()) {
        checkTextureSize();
        CL_FrameBuffer origBuffer = gc.get_write_frame_buffer();

        CL_FrameBuffer fb(gc);
        fb.attach_color_buffer(0, *texture_->getTexture());
        fb.attach_depth_buffer(depthTexture_);

        gc.set_frame_buffer(fb);

        render(gc);

        gc.set_frame_buffer(origBuffer);
    }

    return texture_;
}


void WorldViewRenderer::render(CL_GraphicContext& gc) {
    renderQueue_->preRender();

    gc.clear(CL_Colorf(0.f, 0.f, 0.f, 1.f));
    gc.clear_depth(1.0);

    CL_BufferControl buffer_control;
    buffer_control.set_depth_compare_function(cl_comparefunc_lequal);
    buffer_control.enable_depth_write(false);
    buffer_control.enable_depth_test(false);
    buffer_control.enable_stencil_test(false);
    buffer_control.enable_color_write(true);
    gc.set_buffer_control(buffer_control);

    boost::shared_ptr<CL_ProgramObject> shader = ui::Manager::getShaderManager()->getWorldShader();
    gc.set_program_object(*shader, cl_program_matrix_modelview_projection);

    int clippingLeftPixelCoord = worldView_->getCenterPixelX() - worldView_->getWidth()/2;
    int clippingRightPixelCoord = worldView_->getCenterPixelX() + worldView_->getWidth()/2;
    int clippingTopPixelCoord = worldView_->getCenterPixelY() - worldView_->getHeight()/2;
    int clippingBottomPixelCoord = worldView_->getCenterPixelY() + worldView_->getHeight()/2;
    
    static CL_Rectf texCoordHelper(0.0f, 0.0f, 1.0f, 1.0f);

    static CL_Vec2f texCoords[6] = {
        CL_Vec2f(texCoordHelper.left, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.right, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.left, texCoordHelper.bottom),
        CL_Vec2f(texCoordHelper.right, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.left, texCoordHelper.bottom),
        CL_Vec2f(texCoordHelper.right, texCoordHelper.bottom)
    };

    static CL_Vec2f texCoordsMirrored[6] = {
        CL_Vec2f(texCoordHelper.right, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.left, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.right, texCoordHelper.bottom),
        CL_Vec2f(texCoordHelper.left, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.right, texCoordHelper.bottom),
        CL_Vec2f(texCoordHelper.left, texCoordHelper.bottom)
    };


    CL_Vec2f pixelOffsetVec(clippingLeftPixelCoord, clippingTopPixelCoord);
    shader->set_uniform2f("PositionOffset", pixelOffsetVec);

    gc.set_texture(0, *(data::Manager::getSingleton()->getHuesLoader()->getHuesTexture()->getTexture()));
    shader->set_uniform1i("HueTexture", 0);
    shader->set_uniform1i("ObjectTexture", 1);

    boost::shared_ptr<world::LightManager> lightManager = world::Manager::getLightManager();
    shader->set_uniform3f("AmbientLightIntensity", lightManager->getAmbientIntensity());
    shader->set_uniform3f("GlobalLightIntensity", lightManager->getGlobalIntensity());
    shader->set_uniform3f("GlobalLightDirection", lightManager->getGlobalDirection());

    bool renderingComplete = true;

    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator secIter = world::Manager::getSectorManager()->begin();
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator secEnd = world::Manager::getSectorManager()->end();
    
    unsigned int notInCount = 0;
    unsigned int totalCount = 0;

    for (; secIter != secEnd; ++secIter) {
        std::list<world::IngameObject*>::iterator objIter = secIter->second->renderBegin();
        std::list<world::IngameObject*>::iterator objEnd = secIter->second->renderEnd();
        
        for (; objIter != objEnd; ++objIter) {
            totalCount++;
            world::IngameObject* curObj = *objIter;
        
            // check if current object is in the area visible to the player
            if (!curObj->isInDrawArea(clippingLeftPixelCoord, clippingRightPixelCoord, clippingTopPixelCoord, clippingBottomPixelCoord)) {
                notInCount++;
                continue;
            }

            // check if texture is ready to be drawn
            ui::Texture* tex = curObj->getIngameTexture().get();

            // happens e.g. for the equipped backpack
            if (!tex) {
                continue;
            }

            if (!tex->isReadComplete()) {
                renderingComplete = false;
                continue;
            }

            // object is invisible
            if (!curObj->isVisible()) {
                continue;
            }

            CL_PrimitivesArray primarray(gc);
            primarray.set_attributes(0, curObj->getVertexCoordinates());
            if (curObj->isMirrored()) {
                primarray.set_attributes(1, texCoordsMirrored);
            } else {
                primarray.set_attributes(1, texCoords);
            }
            primarray.set_attributes(2, curObj->getVertexNormals());
            primarray.set_attribute(3, curObj->getHueInfo());

            gc.set_texture(1, *(tex->getTexture()));

            gc.draw_primitives(cl_triangles, 6, primarray);
        }
    }

    gc.reset_textures();
    gc.reset_program_object();

    gc.clear_depth(1.0);

    buffer_control.enable_depth_write(false);
    buffer_control.enable_depth_test(false);
    buffer_control.enable_stencil_test(false);
    buffer_control.enable_color_write(true);
    gc.set_buffer_control(buffer_control);

    renderQueue_->postRender(renderingComplete);



    static unsigned int cnt = 0;
    static boost::shared_ptr<world::ParticleEffect> testEmitter3 = particles::XmlLoader::fromFile("gate");
    
    if (cnt >= 150) {
        testEmitter3->update(20);
        
        // render particle effects
        shader = ui::Manager::getShaderManager()->getParticleShader();
        gc.set_program_object(*shader, cl_program_matrix_modelview_projection);

        CL_Pen defPen = gc.get_pen();
        CL_Pen pen;
        pen.set_point_size(1.0); // TODO: make this dynamic
        pen.enable_vertex_program_point_size(true);
        pen.enable_point_sprite(true);
        gc.set_pen(pen);

        testEmitter3->renderAll(gc, shader);

        //++renderDiff;
        //fluo::sleepMs(20);

        gc.set_pen(defPen);
        gc.reset_program_object();
    }

    ++cnt;
    
    
    LOG_DEBUG << "total: " << totalCount << " not in: " << notInCount << std::endl;
}

boost::shared_ptr<RenderQueue> WorldViewRenderer::getRenderQueue() const {
    return renderQueue_;
}

}
}
