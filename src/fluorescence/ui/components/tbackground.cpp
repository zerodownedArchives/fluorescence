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

#include "tbackground.hpp"

namespace fluo {
namespace ui{
namespace components {

TBackground::TBackground(CL_GUIComponent* parent) : CL_GUIComponent(parent) {
    set_type_name("tbackground");
    themePart_ = CL_GUIThemePart(this);
    
    func_render().set(this, &TBackground::render);
}

void TBackground::render(CL_GraphicContext &gc, const CL_Rect &updateRect) {
    themePart_.render_box(gc, get_size(), updateRect);
}
}
}
}

