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

#include <client.hpp>

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


namespace fluo {
namespace ui {
namespace render {

WorldViewRenderer::WorldViewRenderer(components::WorldView* worldView) : 
        worldView_(worldView), 
        textureWidth_(0), textureHeight_(0), 
        frameBufferIndex_(0), movePixelX_(0), movePixelY_(0), 
        batchFill_(0), forceRepaint_(false) {
    initBufferControls();
    
    boost::shared_ptr<ui::Texture> effTex = data::Manager::getTexture(data::TextureSource::FILE, "effects/textures/rendereffects.png");
    effTex->setUsage(Texture::USAGE_EFFECT);
    if (!effTex) {
        LOG_ERROR << "Error loading the render effect texture" << std::endl;
        throw Exception("Error loading the render effect texture");
    }
    
    // find something better than busy waiting here
    while (!effTex->isReadComplete()) {
        fluo::sleepMs(1);
    }
    renderEffectTexture_ = effTex->getTexture();
}

WorldViewRenderer::~WorldViewRenderer() {
}

void WorldViewRenderer::checkTextureSize() {
    if (colorBuffers_[0].is_null() || textureWidth_ != worldView_->getWidth() || textureHeight_ != worldView_->getHeight()) {
        textureWidth_ = worldView_->getWidth();
        textureHeight_ = worldView_->getHeight();
        
        initFrameBuffer(0);
        initFrameBuffer(1);
    }
}

void WorldViewRenderer::initFrameBuffer(unsigned int index) {
    CL_GraphicContext gc = ui::Manager::getGraphicContext();
    
    colorBuffers_[index] = ui::Manager::getSingleton()->providerRenderBufferTexture(CL_Size(textureWidth_, textureHeight_));
    depthStencilBuffers_[index] = CL_RenderBuffer(gc, textureWidth_, textureHeight_, cl_depth24_stencil8);

    frameBuffers_[index] = CL_FrameBuffer(gc);
    frameBuffers_[index].attach_color_buffer(0, colorBuffers_[index]);
    frameBuffers_[index].attach_depth_stencil_buffer(depthStencilBuffers_[index]);
    
    texturesInitialized_[index] = false;
}

void WorldViewRenderer::moveCenter(float moveX, float moveY) {
    movePixelX_ = moveX;
    movePixelY_ = moveY;
}

CL_Texture WorldViewRenderer::getTexture(CL_GraphicContext& gc) {
    checkTextureSize();
    
    CL_FrameBuffer origBuffer = gc.get_write_frame_buffer();
    frameBufferIndex_ = (frameBufferIndex_ + 1) % 2;
    gc.set_frame_buffer(frameBuffers_[frameBufferIndex_]);
    renderPreviousTexture(gc, movePixelX_, movePixelY_);
    
    CL_Vec2f clippingTopLeftCorner = worldView_->getTopLeftPixel();
    
    ui::ClipRectManager* clipRectMan = ui::Manager::getClipRectManager().get();
    boost::recursive_mutex::scoped_lock clipManLock(clipRectMan->mutex_);
    clipRectMan->clamp(clippingTopLeftCorner, worldView_->getDrawSize());
    
    if (clipRectMan->size() > 0) {
        prepareStencil(gc);
        render(gc);
    } else {
        fluo::sleepMs(1);
    }
    
    gc.set_frame_buffer(origBuffer);
    texturesInitialized_[frameBufferIndex_] = true;
    
    ui::Manager::getClipRectManager()->clear();
    
    return colorBuffers_[frameBufferIndex_];
}

void WorldViewRenderer::renderPreviousTexture(CL_GraphicContext& gc, float pixelX, float pixelY) {
    unsigned int drawWidth = worldView_->getDrawWidth();
    unsigned int drawHeight = worldView_->getDrawHeight();
    
    // ceilfs are to check if the pixel value is even. only then does the clipping stuff work correctly. 
    // pixelX and pixelY can have fractional parts due to zoom
    if (forceRepaint_ || ceilf(pixelX) != pixelX || ceilf(pixelY) != pixelY || abs(pixelX) >= textureWidth_ || abs(pixelY) >= textureHeight_) {
        // need to paint everything from scratch
        ui::Manager::getClipRectManager()->add(CL_Rectf(0, 0, CL_Sizef(drawWidth, drawHeight)).translate(worldView_->getTopLeftPixel()));
        forceRepaint_ = false;
    } else {
        unsigned int oldTexture = (frameBufferIndex_ + 1) % 2;
        
        if (texturesInitialized_[oldTexture]) {
            CL_Rectf geom = worldView_->get_geometry();
            CL_Draw::texture(gc, colorBuffers_[oldTexture], CL_Rectf(-(pixelX + geom.left), -(pixelY + geom.top), CL_Sizef(textureWidth_, textureHeight_)));
            
            if (pixelX < 0) {
                 ui::Manager::getClipRectManager()->add(CL_Rectf(0, 0, CL_Sizef(-pixelX, drawHeight)).translate(worldView_->getTopLeftPixel()));
             } else if (pixelX > 0) {
                 ui::Manager::getClipRectManager()->add(CL_Rectf(drawWidth - pixelX, 0, CL_Sizef(pixelX, drawHeight)).translate(worldView_->getTopLeftPixel()));
             }
             
             if (pixelY < 0) {
                 ui::Manager::getClipRectManager()->add(CL_Rectf(0, 0, CL_Sizef(drawWidth, -pixelY)).translate(worldView_->getTopLeftPixel()));
             } else if (pixelY > 0) {
                  ui::Manager::getClipRectManager()->add(CL_Rectf(0, drawHeight - pixelY, CL_Sizef(drawWidth, pixelY)).translate(worldView_->getTopLeftPixel()));
             }
         } else {
             ui::Manager::getClipRectManager()->add(CL_Rectf(0, 0, CL_Sizef(drawWidth, drawHeight)).translate(worldView_->getTopLeftPixel()));
         }
    }
    
    //LOG_DEBUG << "center moved x=" << pixelX << " y=" << pixelY << std::endl;
}

void WorldViewRenderer::prepareStencil(CL_GraphicContext& gc) {
    gc.clear_stencil(0);
    
    gc.set_buffer_control(bufferControlClips_);
    
    gc.push_modelview();
    gc.set_modelview(CL_Mat4f::identity());
    
    CL_Vec2f clippingTopLeftCorner = worldView_->getTopLeftPixel();
    float zoom = worldView_->getZoom();
    
    ui::ClipRectManager* clipRectMan = ui::Manager::getClipRectManager().get();
    std::vector<CL_Rectf>::const_iterator clipRectIter;
    std::vector<CL_Rectf>::const_iterator clipRectEnd = clipRectMan->end();
    
    for (clipRectIter = clipRectMan->begin(); clipRectIter != clipRectEnd; ++clipRectIter) {
        CL_Rectf clipRect(*clipRectIter);
        
        // need to apply the view matrix manually here, because there is some error with CL_Draw::fill
        clipRect.translate(-clippingTopLeftCorner);
        if (zoom != 1) {
            clipRect.left = floorf(clipRect.left * zoom);
            clipRect.right = ceilf(clipRect.right * zoom);
            clipRect.top = floorf(clipRect.top * zoom);
            clipRect.bottom = ceilf(clipRect.bottom * zoom);
        }
        
        CL_Draw::fill(gc, clipRect.left, clipRect.top, clipRect.right, clipRect.bottom, CL_Colorf::black);
    }
    
    gc.pop_modelview();
}

void WorldViewRenderer::render(CL_GraphicContext& gc) {
    gc.set_buffer_control(bufferControlObjects_);
    
    ui::ClipRectManager* clipRectMan = ui::Manager::getClipRectManager().get();
    
    boost::shared_ptr<CL_ProgramObject> shader = ui::Manager::getShaderManager()->getWorldShader();
    gc.set_program_object(*shader, cl_program_matrix_modelview_projection);
    
    gc.push_modelview();
    gc.set_modelview(worldView_->getViewMatrix());
    
    CL_Texture huesTexture = data::Manager::getHuesLoader()->getHuesTexture();
    gc.set_texture(0, huesTexture);
    gc.set_texture(2, renderEffectTexture_);
    
    // set texture unit 1 active to avoid overriding the hue texture with newly loaded object textures
    gc.set_texture(1, huesTexture);
    
    shader->set_uniform1i("HueTexture", 0);
    shader->set_uniform1i("ObjectTexture", 1);
    shader->set_uniform1i("RenderEffectTexture", 2);

    boost::shared_ptr<world::LightManager> lightManager = world::Manager::getLightManager();
    shader->set_uniform3f("AmbientLightIntensity", lightManager->getAmbientIntensity());
    shader->set_uniform3f("GlobalLightIntensity", lightManager->getGlobalIntensity());
    shader->set_uniform3f("GlobalLightDirection", lightManager->getGlobalDirection());
    
    timeval t = Client::getSingleton()->getElapsedTime();
    float renderEffectTime = t.tv_sec + (t.tv_usec / 1000000.0);
    shader->set_uniform1f("RenderEffectTime", renderEffectTime);

    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator secIter = world::Manager::getSectorManager()->begin();
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator secEnd = world::Manager::getSectorManager()->end();
    
    std::list<world::IngameObject*>::iterator objIter;
    std::list<world::IngameObject*>::iterator objEnd;;
    
    for (; secIter != secEnd; ++secIter) {
        // TOOD: check if we can skip the whole sector
        
        objIter = secIter->second->renderBegin();
        objEnd = secIter->second->renderEnd();
        
        for (; objIter != objEnd; ++objIter) {
            world::IngameObject* curObj = *objIter;
            
            // check if texture is ready to be drawn
            ui::Texture* tex = curObj->getIngameTexture().get();

            if (!tex || !tex->isReadComplete() || !curObj->isVisible()) {
                continue;
            }
        
            if (clipRectMan->overlapsAny(curObj)) {
                renderObject(gc, curObj, tex);
            }
        }
    }
    
    batchFlush(gc);
    
    gc.pop_modelview();
}

void WorldViewRenderer::renderParticleEffects(CL_GraphicContext& gc) {
    CL_Pen defPen = gc.get_pen();
    CL_Pen pen;
    pen.enable_vertex_program_point_size(true);
    pen.enable_point_sprite(true);
    gc.set_pen(pen);
    
    gc.set_buffer_control(bufferControlParticles_);
    
    glEnable(0x8861); // GL_POINT_SPRITE
    
    boost::shared_ptr<CL_ProgramObject> shader = ui::Manager::getShaderManager()->getParticleShader();
    gc.set_program_object(*shader, cl_program_matrix_modelview_projection);
    
    gc.push_modelview();
    gc.set_modelview(worldView_->getViewMatrix());
    
    std::list<boost::shared_ptr<world::Effect> >::iterator particleIter = world::Manager::getSingleton()->effectsBegin();
    std::list<boost::shared_ptr<world::Effect> >::iterator particleEnd = world::Manager::getSingleton()->effectsEnd();

    for (; particleIter != particleEnd; ++particleIter) {
        if ((*particleIter)->isParticleEffect()) {
            boost::static_pointer_cast<world::ParticleEffect>(*particleIter)->renderAll(gc, shader);
        }
    }
    
    gc.set_pen(defPen);
    gc.reset_program_object();
    
    gc.pop_modelview();
}

void WorldViewRenderer::renderObject(CL_GraphicContext& gc, world::IngameObject* obj, ui::Texture* tex) {
    CL_Rectf texCoordHelper = tex->getNormalizedTextureCoords();

    if (lastTexture_ != tex->getTexture()) {
        batchFlush(gc);
        
        lastTexture_ = tex->getTexture();
    }
    
    memcpy(&batchPositions_[batchFill_], obj->getVertexCoordinates(), sizeof(CL_Vec3f) * 6);
    memcpy(&batchNormals_[batchFill_], obj->getVertexNormals(), sizeof(CL_Vec3f) * 6);

    if (obj->isMirrored()) {
        CL_Vec2f texCoordsMirrored[6] = {
            CL_Vec2f(texCoordHelper.right, texCoordHelper.top),
            CL_Vec2f(texCoordHelper.left, texCoordHelper.top),
            CL_Vec2f(texCoordHelper.right, texCoordHelper.bottom),
            CL_Vec2f(texCoordHelper.left, texCoordHelper.top),
            CL_Vec2f(texCoordHelper.right, texCoordHelper.bottom),
            CL_Vec2f(texCoordHelper.left, texCoordHelper.bottom)
        };
        memcpy(&batchTexCoords_[batchFill_], texCoordsMirrored, sizeof(CL_Vec2f) * 6);
    } else {
        CL_Vec2f texCoords[6] = {
            CL_Vec2f(texCoordHelper.left, texCoordHelper.top),
            CL_Vec2f(texCoordHelper.right, texCoordHelper.top),
            CL_Vec2f(texCoordHelper.left, texCoordHelper.bottom),
            CL_Vec2f(texCoordHelper.right, texCoordHelper.top),
            CL_Vec2f(texCoordHelper.left, texCoordHelper.bottom),
            CL_Vec2f(texCoordHelper.right, texCoordHelper.bottom)
        };
        memcpy(&batchTexCoords_[batchFill_], texCoords, sizeof(CL_Vec2f) * 6);
    }

    CL_Vec3f hueInfo = obj->getHueInfo();
    float effect = obj->getRenderEffect();
    for (unsigned int i = 0; i < 6; ++i) {
        batchHueInfos_[batchFill_ + i] = hueInfo;
        // there were some weird issues with automatic conversion from float to CL_Vec1f
        batchRenderEffects_[batchFill_ + i].x = effect; 
    }
    
    batchFill_ += 6;
    
    if (batchFill_ == BATCH_NUM_VERTICES) {
        batchFlush(gc);
    }
}

void WorldViewRenderer::batchFlush(CL_GraphicContext& gc) {
    if (batchFill_ > 0 && !lastTexture_.is_null()) {
        //LOG_DEBUG << "batch flush: " << (batchFill_ / 6) << std::endl;
        CL_PrimitivesArray primarray(gc);
        primarray.set_attributes(0, batchPositions_);
        primarray.set_attributes(1, batchTexCoords_);
        primarray.set_attributes(2, batchNormals_);
        primarray.set_attributes(3, batchHueInfos_);
        primarray.set_attributes(4, batchRenderEffects_);

        gc.set_texture(1, lastTexture_);

        gc.draw_primitives(cl_triangles, batchFill_, primarray);

        batchFill_ = 0;
    }
}

void WorldViewRenderer::forceRepaint() {
    forceRepaint_ = true;
    texturesInitialized_[0] = false;
    texturesInitialized_[1] = false;
}

void WorldViewRenderer::initBufferControls() {
    // first, we want to set the stencil buffer to 1 in the clipped areas. depth is ignored
    bufferControlClips_.enable_depth_write(false);
    bufferControlClips_.enable_depth_test(false);
    bufferControlClips_.enable_color_write(true); // true because the background black is drawn at the same time
    bufferControlClips_.enable_stencil_test(true);
    bufferControlClips_.enable_logic_op(false);
    bufferControlClips_.set_stencil_compare_func(cl_comparefunc_always, cl_comparefunc_always);
    bufferControlClips_.set_stencil_compare_mask(1, 1);
    bufferControlClips_.set_stencil_compare_reference(1, 1);
    bufferControlClips_.set_stencil_write_mask(1, 1);
    bufferControlClips_.set_stencil_fail(cl_stencil_keep, cl_stencil_keep);
    bufferControlClips_.set_stencil_pass_depth_fail(cl_stencil_replace, cl_stencil_replace);
    bufferControlClips_.set_stencil_pass_depth_pass(cl_stencil_replace, cl_stencil_replace);
    
    // ingame objects are only drawn over pixels that are == 1 in the stencil buffer. depth is ignored
    bufferControlObjects_.enable_depth_write(false);
    bufferControlObjects_.enable_depth_test(false);
    bufferControlObjects_.enable_color_write(true);
    bufferControlObjects_.enable_stencil_test(true);
    bufferControlObjects_.enable_logic_op(false);
    bufferControlObjects_.set_stencil_compare_func(cl_comparefunc_equal, cl_comparefunc_equal);
    bufferControlObjects_.set_stencil_compare_mask(1, 1);
    bufferControlObjects_.set_stencil_compare_reference(1, 1);
    bufferControlObjects_.set_stencil_write_mask(1, 1);
    bufferControlObjects_.set_stencil_fail(cl_stencil_keep, cl_stencil_keep);
    bufferControlObjects_.set_stencil_pass_depth_fail(cl_stencil_keep, cl_stencil_keep);
    bufferControlObjects_.set_stencil_pass_depth_pass(cl_stencil_keep, cl_stencil_keep);
    
    // particles are just drawn on top without any checks
    bufferControlParticles_.enable_depth_write(false);
    bufferControlParticles_.enable_depth_test(false);
    bufferControlParticles_.enable_stencil_test(false);
    bufferControlParticles_.enable_color_write(true);
}

}
}
}
