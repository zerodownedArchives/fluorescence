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

#include "emittable.hpp"

namespace fluo {
namespace ui {
namespace particles {

class ParticleEmitter;

class Particle : public Emittable {

friend class ParticleEmitter;

public:
    Particle();

    Particle(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, const CL_Vec4f colStart, const CL_Vec4f& colEnd);
    void reset(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, const CL_Vec4f colStart, const CL_Vec4f& colEnd);

    bool isRemoved() const;

    // does not destroy the particle, but moves it out of sight
    void remove();

protected:
    CL_Vec4f colorStart_;
    CL_Vec4f colorEnd_;

    bool isRemoved_;
};

}
}
}

#endif
