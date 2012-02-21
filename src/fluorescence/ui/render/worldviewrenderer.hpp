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

#include <boost/shared_ptr.hpp>

#include <typedefs.hpp>
#include <ui/render/ingameobjectrenderer.hpp>

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

class WorldViewRenderer : public IngameObjectRenderer {
public:
    WorldViewRenderer(components::WorldView* ingameView);
    ~WorldViewRenderer();

    virtual boost::shared_ptr<Texture> getTexture(CL_GraphicContext& gc, float moveX, float moveY);
    
    virtual boost::shared_ptr<RenderQueue> getRenderQueue() const;

private:
    components::WorldView* worldView_;
    
    virtual void render(CL_GraphicContext& gc);
    void renderObject(CL_GraphicContext& gc, world::IngameObject* obj, ui::Texture* tex) const;

    unsigned int textureWidth_;
    unsigned int textureHeight_;
    boost::shared_ptr<Texture> textures_[2];
    CL_Texture depthTextures_[2];
    void checkTextureSize();
    
    unsigned int frameBufferIndex_;
    void initFrameBuffer(unsigned int index);
    CL_FrameBuffer frameBuffers_[2];
    bool texturesInitialized_[2];
    
    // draws the texture from the last frame, moved x/y pixels
    void renderPreviousTexture(CL_GraphicContext& gc, float pixelX, float pixelY);
};

}
}
}

#endif
