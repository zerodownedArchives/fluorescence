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


#ifndef FLUO_UI_WORLDVIEWRENDERER_HPP
#define FLUO_UI_WORLDVIEWRENDERER_HPP

#include <ClanLib/Display/Render/graphic_context.h>
#include <ClanLib/Display/Render/texture.h>
#include <ClanLib/Display/Render/render_buffer.h>
#include <ClanLib/Display/Render/buffer_control.h>

#include <boost/shared_ptr.hpp>

#include <typedefs.hpp>

namespace fluo {

namespace world {
class IngameObject;
}

namespace ui {

class Texture;

namespace components {
class WorldView;
}

namespace render {

class WorldViewRenderer {
public:
    WorldViewRenderer(components::WorldView* ingameView);
    ~WorldViewRenderer();

    void moveCenter(float moveX, float moveY);
    CL_Texture getTexture(CL_GraphicContext& gc);
    
    void renderParticleEffects(CL_GraphicContext& gc);
    
    void forceRepaint();

private:
    components::WorldView* worldView_;
    
    virtual void render(CL_GraphicContext& gc);
    void renderObject(CL_GraphicContext& gc, world::IngameObject* obj, ui::Texture* tex);
    
    unsigned int textureWidth_;
    unsigned int textureHeight_;
    void checkTextureSize();
    
    CL_Texture colorBuffers_[2];
    CL_RenderBuffer depthStencilBuffers_[2];
    unsigned int frameBufferIndex_;
    void initFrameBuffer(unsigned int index);
    CL_FrameBuffer frameBuffers_[2];
    bool texturesInitialized_[2];
    
    // draws the texture from the last frame, moved x/y pixels
    void renderPreviousTexture(CL_GraphicContext& gc, float pixelX, float pixelY);
    
    float movePixelX_;
    float movePixelY_;
    
    static const unsigned int BATCH_NUM_VERTICES = 600; // render up to 100 objects at the same time
    CL_Vec3f batchPositions_[BATCH_NUM_VERTICES];
    CL_Vec3f batchNormals_[BATCH_NUM_VERTICES];
    CL_Vec2f batchTexCoords_[BATCH_NUM_VERTICES];
    CL_Vec3f batchHueInfos_[BATCH_NUM_VERTICES];
    CL_Vec1f batchRenderEffects_[BATCH_NUM_VERTICES];

    CL_Texture lastTexture_;
    unsigned int batchFill_;
    void batchFlush(CL_GraphicContext& gc);
    
    void prepareStencil(CL_GraphicContext& gc);
    
    bool forceRepaint_;
    
    void initBufferControls();
    CL_BufferControl bufferControlClips_;
    CL_BufferControl bufferControlObjects_;
    CL_BufferControl bufferControlParticles_;
    
    CL_Texture renderEffectTexture_;
};

}
}
}

#endif
