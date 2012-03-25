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



#include "particle.hpp"

namespace fluo {
namespace ui {
namespace particles {

Particle::Particle() : colorStart_(0, 0, 0, 0), colorEnd_(0, 0, 0, 0), isRemoved_(false) {
}

Particle::Particle(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, const CL_Vec4f colStart, const CL_Vec4f& colEnd) :
        Emittable(startPos, velStart, velEnd, creationTime, expireTime),
        colorStart_(colStart), colorEnd_(colEnd) {
}

void Particle::reset(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, const CL_Vec4f colStart, const CL_Vec4f& colEnd) {
    Emittable::reset(startPos, velStart, velEnd, creationTime, expireTime);
    colorStart_ = colStart;
    colorEnd_ = colEnd;
}

bool Particle::isRemoved() const {
    return isRemoved_;
}

void Particle::remove() {
    isRemoved_ = true;
    startLocation_.z = 10.0;
}

}
}
}
