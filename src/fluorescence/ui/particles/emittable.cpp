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



#include "emittable.hpp"

#include <stdlib.h>

namespace fluo {
namespace ui {
namespace particles {

Emittable::Emittable() :
    startPosition_(0, 0, 0), velocityStart_(0, 0, 0), velocityEnd_(0, 0, 0), lifetimes_(0, 0) {
}

Emittable::Emittable(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime) :
    startPosition_(startPos), velocityStart_(velStart), velocityEnd_(velEnd), lifetimes_(creationTime, expireTime) {
}

void Emittable::reset(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime) {
    startPosition_ = startPos;
    velocityStart_ = velStart;
    velocityEnd_ = velEnd;
    lifetimes_ = CL_Vec2f(creationTime, expireTime);
}

bool Emittable::isExpired(float emitterAge) const {
    return emitterAge > lifetimes_[1u];
}

}
}
}
