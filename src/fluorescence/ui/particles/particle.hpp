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


#ifndef FLUO_UI_PARTICLES_PARTICLE_HPP
#define FLUO_UI_PARTICLES_PARTICLE_HPP

#include <ClanLib/Core/Math/vec4.h>

namespace fluo {
namespace ui {
namespace particles {

class Particle {
public:
    bool isExpired(float emitterAge) const {
        return emitterAge > lifetimes_[1u];
    }
    
    CL_Vec3f startLocation_;

    CL_Vec3f velocityStart_;
    CL_Vec3f velocityEnd_;

    CL_Vec2f lifetimes_; // 0 - creation, 1 - expiration

    CL_Vec4f colorStart_;
    CL_Vec4f colorEnd_;
    CL_Vec2f sizes_; // 0 - start, 1 - end

    bool isRemoved_;
};

}
}
}

#endif
