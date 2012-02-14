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

class RenderQueue;
class Texture;

namespace components {
class WorldView;
}

class WorldViewRenderer : public IngameObjectRenderer {
public:
    WorldViewRenderer(boost::shared_ptr<RenderQueue> renderQueue, components::WorldView* ingameView);
    ~WorldViewRenderer();

    virtual boost::shared_ptr<Texture> getTexture(CL_GraphicContext& gc);
    virtual void render(CL_GraphicContext& gc);

    virtual boost::shared_ptr<RenderQueue> getRenderQueue() const;

private:
    components::WorldView* worldView_;
    boost::shared_ptr<RenderQueue> renderQueue_;

    boost::shared_ptr<Texture> texture_;
    CL_Texture depthTexture_;
    void checkTextureSize();
    
    // draw more pixels than currently displayed (borderSize/2 more pixels in every direction)
    static const unsigned int BORDER_SIZE_HALF = 50;
    static const unsigned int BORDER_SIZE = BORDER_SIZE_HALF * 2;
    
    CL_FrameBuffer frameBuffer_;
};

}
}

#endif
