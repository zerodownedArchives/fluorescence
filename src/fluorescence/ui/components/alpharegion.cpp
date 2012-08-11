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

#include "alpharegion.hpp"

#include <ClanLib/Display/2D/draw.h>
#include <ClanLib/Display/Render/blend_mode.h>

#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace components {

AlphaRegion::AlphaRegion(CL_GUIComponent* parent) : GumpComponent(parent), alpha_(1.0) {
    func_render().set(this, &AlphaRegion::render);

    set_type_name("alpharegion");
}

void AlphaRegion::setAlpha(float alpha) {
    alpha_ = alpha;
}

void AlphaRegion::render(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    CL_BlendMode oldBlend = gc.get_blend_mode();

    CL_BlendMode newBlend;
    if (alpha_ == 0) {
        // special case for full transparency
        newBlend.set_blend_function(cl_blend_zero, cl_blend_zero, cl_blend_zero, cl_blend_zero);
    } else {
        newBlend.set_blend_function(cl_blend_zero, cl_blend_one, cl_blend_zero, cl_blend_src_alpha);
    }
    gc.set_blend_mode(newBlend);
    CL_Draw::fill(gc, CL_Rectf(0, 0, get_width(), get_height()), CL_Colorf(0.f, 0.f, 0.f, alpha_));

    // restore old mode
    gc.set_blend_mode(oldBlend);
}

}
}
}

