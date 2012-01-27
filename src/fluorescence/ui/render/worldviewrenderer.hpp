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


    // render batching
    static const unsigned int BATCH_NUM_VERTICES = 600; // render up to 100 objects at the same time
    CL_Vec3f batchPositions_[BATCH_NUM_VERTICES];
    CL_Vec3f batchNormals_[BATCH_NUM_VERTICES];
    CL_Vec2f batchTexCoords_[BATCH_NUM_VERTICES];
    CL_Vec3f batchHueInfos_[BATCH_NUM_VERTICES];

    unsigned int batchFill_;
    ui::Texture* lastTexture_;

    void batchAdd(CL_GraphicContext& gc, world::IngameObject* curObj, ui::Texture* tex, unsigned long long& minRenderPrio, float renderDiff);
    void batchFlush(CL_GraphicContext& gc);
};

}
}

#endif
