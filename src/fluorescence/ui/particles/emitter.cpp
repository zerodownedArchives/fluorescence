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



#include "emitter.hpp"

#include <algorithm>

#include "startlocationprovider.hpp"
#include "motionmodel.hpp"

namespace fluo {
namespace ui {
namespace particles {
    
Emitter::Emitter() : 
        age_(0), emittedFractionStore_(0.99999f), emittedMoveWithEmitter_(false) {
    // emittedFractionStore is set to emit at least one child at the start
}

void Emitter::setLocation(const CL_Vec3f& location) {
    location_ = location;
}

void Emitter::setLocationOffset(const CL_Vec3f& offset) {
    locationOffset_ = offset;
}

CL_Vec3f Emitter::getLocation() const {
    return location_ + locationOffset_;
}

CL_Vec3f Emitter::getStartLocation() const {
    return startLocation_ + locationOffset_;
}

}
}
}
