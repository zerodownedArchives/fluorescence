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


#ifndef FLUO_UI_INGAMEOBJECTRENDERER_HPP
#define FLUO_UI_INGAMEOBJECTRENDERER_HPP

#include <boost/shared_ptr.hpp>
#include <ClanLib/Display/Render/graphic_context.h>

namespace fluo {
namespace ui {

class Texture;
class RenderQueue;

class IngameObjectRenderer {
public:
    enum {
        TYPE_WORLD, // render the world "as-is", with all objects, including speech etc
        TYPE_GUMP, // render gump images, e.g. paperdoll
    };

    IngameObjectRenderer(unsigned int rendererType);

    // get the rendered objects as a texture. can be used i.e. for postprocessing or for a cached texture that does not change very often
    virtual boost::shared_ptr<Texture> getTexture(CL_GraphicContext& gc) = 0;

    // render the objects to the current state of the gc
    virtual void render(CL_GraphicContext& gc) = 0;


    bool isWorldRenderer() const;
    bool isGumpRenderer() const;

    virtual boost::shared_ptr<RenderQueue> getRenderQueue() const = 0;

    // due to some strange clanlib behaviour. when opening a paperdoll for the first time, the hue texture seems
    // not to be loaded correctly, so the gump is mostly black (with outlines). When rendering it again, all seems
    // fine. this flag indicates wheter the renderer has to re-render the image because of this condition
    bool requireInitialRepaint();

private:
    unsigned int rendererType_;
    bool requireInitialRepaint_;
};

}
}


#endif
