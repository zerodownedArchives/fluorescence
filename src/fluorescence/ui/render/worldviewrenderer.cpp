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
#include <ui/render/shadermanager.hpp>
#include <ui/texture.hpp>
#include <ui/components/worldview.hpp>
#include <ui/cliprectmanager.hpp>

#include <misc/log.hpp>

#include <world/ingameobject.hpp>
#include <world/lightmanager.hpp>
#include <world/manager.hpp>
#include <world/particleeffect.hpp>
#include <world/sectormanager.hpp>
#include <world/sector.hpp>
#include <world/dynamicitem.hpp>

#include <data/manager.hpp>
#include <data/huesloader.hpp>



#include <ui/particles/xmlloader.hpp>

namespace fluo {
namespace ui {
namespace render {

WorldViewRenderer::WorldViewRenderer(components::WorldView* worldView) : IngameObjectRenderer(IngameObjectRenderer::TYPE_WORLD),
        worldView_(worldView), textureWidth_(0), textureHeight_(0), frameBufferIndex_(0), movePixelX_(0), movePixelY_(0) {
}

WorldViewRenderer::~WorldViewRenderer() {
}

void WorldViewRenderer::checkTextureSize() {
    if (!textures_[0] || textureWidth_ != worldView_->getWidth() || textureHeight_ != worldView_->getHeight()) {
        textureWidth_ = worldView_->getWidth();
        textureHeight_ = worldView_->getHeight();
        
        initFrameBuffer(0);
        initFrameBuffer(1);
    }
}

void WorldViewRenderer::initFrameBuffer(unsigned int index) {
    textures_[index].reset(new ui::Texture(false));
    textures_[index]->initPixelBuffer(textureWidth_, textureHeight_);
    textures_[index]->setReadComplete();

    CL_GraphicContext gc = ui::Manager::getGraphicContext();

    depthTextures_[index] = CL_Texture(gc, textureWidth_, textureHeight_, cl_depth_component);
    
    frameBuffers_[index] = CL_FrameBuffer(gc);
    frameBuffers_[index].attach_color_buffer(0, *textures_[index]->getTexture());
    frameBuffers_[index].attach_depth_buffer(depthTextures_[index]);
    
    texturesInitialized_[index] = false;
}

void WorldViewRenderer::moveCenter(float moveX, float moveY) {
    movePixelX_ = moveX;
    movePixelY_ = moveY;
}

boost::shared_ptr<Texture> WorldViewRenderer::getTexture(CL_GraphicContext& gc) {
    checkTextureSize();
    CL_FrameBuffer origBuffer = gc.get_write_frame_buffer();
    
    frameBufferIndex_ = (frameBufferIndex_ + 1) % 2;

    gc.set_frame_buffer(frameBuffers_[frameBufferIndex_]);
    
    renderPreviousTexture(gc, movePixelX_, movePixelY_);

    render(gc);
    
    gc.set_frame_buffer(origBuffer);
    
    texturesInitialized_[frameBufferIndex_] = true;
    return textures_[frameBufferIndex_];
}

void WorldViewRenderer::renderPreviousTexture(CL_GraphicContext& gc, float pixelX, float pixelY) {
    if (abs(pixelX) >= textureWidth_ || abs(pixelY) >= textureHeight_) {
        // need to paint everything from scratch
        ui::Manager::getClipRectManager()->add(CL_Rectf(0, 0, CL_Sizef(textureWidth_, textureHeight_)).translate(worldView_->getTopLeftPixel()));
    } else {
        unsigned int oldTexture = (frameBufferIndex_ + 1) % 2;
        
        if (texturesInitialized_[oldTexture]) {
            gc.clear_depth(1.0);
            CL_Draw::texture(gc, *textures_[oldTexture]->getTexture(), CL_Rectf(-pixelX, -pixelY, CL_Sizef(textureWidth_, textureHeight_)));
            
            if (pixelX < 0) {
                ui::Manager::getClipRectManager()->add(CL_Rectf(0, 0, CL_Sizef(-pixelX, textureHeight_)).translate(worldView_->getTopLeftPixel()));
            } else if (pixelX > 0) {
                ui::Manager::getClipRectManager()->add(CL_Rectf(textureWidth_ - pixelX, 0, CL_Sizef(pixelX, textureHeight_)).translate(worldView_->getTopLeftPixel()));
            }
            
            if (pixelY < 0) {
                ui::Manager::getClipRectManager()->add(CL_Rectf(0, 0, CL_Sizef(textureWidth_, -pixelY)).translate(worldView_->getTopLeftPixel()));
            } else if (pixelY > 0) {
                ui::Manager::getClipRectManager()->add(CL_Rectf(0, textureHeight_ - pixelY, CL_Sizef(textureWidth_, pixelY)).translate(worldView_->getTopLeftPixel()));
            }
        } else {
            ui::Manager::getClipRectManager()->add(CL_Rectf(0, 0, CL_Sizef(worldView_->getWidth(), worldView_->getHeight())).translate(worldView_->getTopLeftPixel()));
        }
    }
    
    //LOG_DEBUG << "center moved x=" << pixelX << " y=" << pixelY << std::endl;
}


void WorldViewRenderer::render(CL_GraphicContext& gc) {
    CL_BufferControl buffer_control;
    buffer_control.set_depth_compare_function(cl_comparefunc_lequal);
    buffer_control.enable_depth_write(false);
    buffer_control.enable_depth_test(false);
    buffer_control.enable_stencil_test(false);
    buffer_control.enable_color_write(true);
    gc.set_buffer_control(buffer_control);
    
    CL_Vec2f clippingTopLeftCorner = worldView_->getTopLeftPixel();
    
    ui::ClipRectManager* clipRectMan = ui::Manager::getClipRectManager().get();
    boost::mutex::scoped_lock clipManLock(clipRectMan->mutex_);
    clipRectMan->clamp(clippingTopLeftCorner, worldView_->get_size());
    
    if (clipRectMan->size() == 0) {
        fluo::sleepMs(1);
        return;
    }
    
    std::vector<CL_Rectf>::const_iterator clipRectIter;
    std::vector<CL_Rectf>::const_iterator clipRectEnd = clipRectMan->end();
    
    for (clipRectIter = clipRectMan->begin(); clipRectIter != clipRectEnd; ++clipRectIter) {
        CL_Rectf clipRect(*clipRectIter);
        clipRect.translate(-clippingTopLeftCorner);
        gc.push_cliprect(clipRect);
        gc.clear(CL_Colorf::black);
        gc.pop_cliprect();
    }

    boost::shared_ptr<CL_ProgramObject> shader = ui::Manager::getShaderManager()->getWorldShader();
    gc.set_program_object(*shader, cl_program_matrix_modelview_projection);
    
    shader->set_uniform2f("PositionOffset", clippingTopLeftCorner);
    
    boost::shared_ptr<ui::Texture> huesTexture = data::Manager::getSingleton()->getHuesLoader()->getHuesTexture();
    gc.set_texture(0, *(huesTexture->getTexture()));
    // set texture unit 1 active to avoid overriding the hue texture with newly loaded object textures
    gc.set_texture(1, *(huesTexture->getTexture())); 
    
    shader->set_uniform1i("HueTexture", 0);
    shader->set_uniform1i("ObjectTexture", 1);

    boost::shared_ptr<world::LightManager> lightManager = world::Manager::getLightManager();
    shader->set_uniform3f("AmbientLightIntensity", lightManager->getAmbientIntensity());
    shader->set_uniform3f("GlobalLightIntensity", lightManager->getGlobalIntensity());
    shader->set_uniform3f("GlobalLightDirection", lightManager->getGlobalDirection());

    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator secIter = world::Manager::getSectorManager()->begin();
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator secEnd = world::Manager::getSectorManager()->end();
    
    std::list<world::IngameObject*>::iterator objIter;
    std::list<world::IngameObject*>::iterator objEnd;;

    unsigned int notInCount = 0;
    unsigned int totalCount = 0;
    
    for (; secIter != secEnd; ++secIter) {
        // TOOD: check if we can skip the whole sector
        
        objIter = secIter->second->renderBegin();
        objEnd = secIter->second->renderEnd();
        
        for (; objIter != objEnd; ++objIter) {
            totalCount++;
            world::IngameObject* curObj = *objIter;
            
            // check if texture is ready to be drawn
            ui::Texture* tex = curObj->getIngameTexture().get();
            
            if (tex && !tex->isReadComplete() && curObj->isDynamicItem() && ((world::DynamicItem*)(curObj))->getLayer() == Layer::MOUNT) {
                LOG_ERROR << "\n\nmount with incomplete texture\n\n" << std::endl;
            }

            if (!tex || !tex->isReadComplete() || !curObj->isVisible()) {
                continue;
            }
        
            // check if current object is in the area visible to the player
            bool drawn = false;
            for (clipRectIter = clipRectMan->begin(); clipRectIter != clipRectEnd; ++clipRectIter) {
                if (curObj->overlaps(*clipRectIter)) {
                    CL_Rectf clipRect(*clipRectIter);
                    clipRect.translate(-clippingTopLeftCorner);
                    gc.push_cliprect(clipRect);
                    //gc.set_texture(0, *(huesTexture->getTexture()));
                    renderObject(gc, curObj, tex);
                    drawn = true;
                    gc.pop_cliprect();
                }
                
                if (!drawn) {
                    notInCount++;
                }
            }
        }
    }
    
    gc.reset_program_object();
    gc.reset_textures();
    
    ui::Manager::getClipRectManager()->clear();
    
    //buffer_control.enable_depth_write(false);
    //buffer_control.enable_depth_test(false);
    //buffer_control.enable_stencil_test(false);
    //buffer_control.enable_color_write(true);
    //gc.set_buffer_control(buffer_control);


    //static unsigned int cnt = 0;
    //static boost::shared_ptr<world::ParticleEffect> testEmitter3 = particles::XmlLoader::fromFile("gate");
    
    //if (cnt >= 150) {
        //testEmitter3->update(20);
        
        //// render particle effects
        //shader = ui::Manager::getShaderManager()->getParticleShader();
        //gc.set_program_object(*shader, cl_program_matrix_modelview_projection);

        //CL_Pen defPen = gc.get_pen();
        //CL_Pen pen;
        //pen.set_point_size(1.0); // TODO: make this dynamic
        //pen.enable_vertex_program_point_size(true);
        //pen.enable_point_sprite(true);
        //gc.set_pen(pen);

        //testEmitter3->renderAll(gc, shader);

        ////++renderDiff;
        ////fluo::sleepMs(20);

        //gc.set_pen(defPen);
        //gc.reset_program_object();
    //}

    //++cnt;
    
    
    if (totalCount > 0) {
        //LOG_DEBUG << "total: " << totalCount << " not in: " << notInCount << std::endl;
    }
}

boost::shared_ptr<RenderQueue> WorldViewRenderer::getRenderQueue() const {
    boost::shared_ptr<RenderQueue> rq;
    return rq;
}

void WorldViewRenderer::renderObject(CL_GraphicContext& gc, world::IngameObject* obj, ui::Texture* tex) const {
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
    
    CL_PrimitivesArray primarray(gc);
    primarray.set_attributes(0, obj->getVertexCoordinates());
    if (obj->isMirrored()) {
        primarray.set_attributes(1, texCoordsMirrored);
    } else {
        primarray.set_attributes(1, texCoords);
    }
    primarray.set_attributes(2, obj->getVertexNormals());
    primarray.set_attribute(3, obj->getHueInfo());

    gc.set_texture(1, *(tex->getTexture()));

    gc.draw_primitives(cl_triangles, 6, primarray);
}

}
}
}
