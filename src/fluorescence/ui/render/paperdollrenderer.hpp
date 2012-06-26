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


#ifndef FLUO_UI_PAPERDOLLRENDERER_HPP
#define FLUO_UI_PAPERDOLLRENDERER_HPP

#include <ClanLib/Display/Render/graphic_context.h>
#include <ClanLib/Display/Render/texture.h>

#include <boost/shared_ptr.hpp>

namespace fluo {
namespace ui {

namespace components {
class PaperdollView;
}

class RenderQueue;
class Texture;

class PaperdollRenderer {
public:
    PaperdollRenderer(boost::shared_ptr<RenderQueue> renderQueue, components::PaperdollView* pdView);
    ~PaperdollRenderer();

    CL_Texture getTexture(CL_GraphicContext& gc);

    void render(CL_GraphicContext& gc);

    boost::shared_ptr<RenderQueue> getRenderQueue() const;

private:
    components::PaperdollView* paperdollView_;
    boost::shared_ptr<RenderQueue> renderQueue_;

    CL_Texture texture_;
    void checkTextureSize(CL_GraphicContext& gc);

    CL_FrameBuffer frameBuffer_;
};
}
}

#endif
