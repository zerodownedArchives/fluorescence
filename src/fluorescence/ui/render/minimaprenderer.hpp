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


#ifndef FLUO_UI_MINIMAPRENDERER_HPP
#define FLUO_UI_MINIMAPRENDERER_HPP

#include <ClanLib/Display/Render/graphic_context.h>
#include <ClanLib/Display/Render/texture.h>
#include <ClanLib/Display/Render/render_buffer.h>
#include <ClanLib/Display/Render/buffer_control.h>

#include <boost/shared_ptr.hpp>

#include <typedefs.hpp>

namespace fluo {

namespace world {
class MiniMapBlock;
}

namespace ui {

class Texture;

namespace components {
class MiniMapView;
}

namespace render {

class MiniMapRenderer {
public:
    MiniMapRenderer(components::MiniMapView* ingameView);
    ~MiniMapRenderer();

    void moveCenter(float moveX, float moveY);
    CL_Texture getTexture(CL_GraphicContext& gc);

    void forceRepaint();

private:
    components::MiniMapView* miniMapView_;

    virtual void render(CL_GraphicContext& gc);

    CL_Texture texture_;
    void checkTextureSize(CL_GraphicContext& gc);

    CL_FrameBuffer frameBuffer_;

    static const unsigned int BATCH_NUM_VERTICES = 60; // render up to 10 objects at the same time
    CL_Vec3f batchPositions_[BATCH_NUM_VERTICES];
    CL_Vec2f batchTexCoords_[BATCH_NUM_VERTICES];
    CL_Vec3f batchHueInfos_[BATCH_NUM_VERTICES];

    CL_Texture lastTexture_;
    unsigned int batchFill_;
    void batchFlush(CL_GraphicContext& gc);
    void renderMiniBlock(CL_GraphicContext& gc, world::MiniMapBlock* block);

    bool forceRepaint_;

    void initBufferControls();
    CL_BufferControl bufferControlObjects_;
};

}
}
}

#endif
