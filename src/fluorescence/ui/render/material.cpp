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

#include "material.hpp"

#include <typedefs.hpp>

namespace fluo {
namespace ui {
namespace render {

MaterialInfo MaterialInfo::materials_[2] = { 
    MaterialInfo(Material::DEFAULT, false),
    MaterialInfo(Material::WATER, true),
};

const MaterialInfo* MaterialInfo::get(unsigned int material) {
    if (material > 1) {
        material = 0;
    }
    
    return &materials_[material];
}

MaterialInfo::MaterialInfo(unsigned int id, bool repaint) : id_(id), constantRepaint_(repaint) {
}
   
}
}
}

