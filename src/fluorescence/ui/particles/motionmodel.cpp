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



#include "motionmodel.hpp"

namespace fluo {
namespace ui {
namespace particles {

void MotionModelStatic::get(const CL_Vec3f& emitterLocation, const CL_Vec3f& particleLocation, CL_Vec3f& outParam1, CL_Vec3f& outParam2) const {
    outParam1 = CL_Vec3f(0, 0, 0);
    outParam2 = CL_Vec3f(0, 0, 0);
}


void MotionModelStartEndVelocity::setVelocitiesStart(const CL_Vec3f& min, const CL_Vec3f& max) {
    startVelocity_.set(max, min);
}

void MotionModelStartEndVelocity::setVelocitiesEnd(const CL_Vec3f& min, const CL_Vec3f& max) {
    endVelocity_.set(max, min);
}

void MotionModelStartEndVelocity::setVelocityAndAccelerationStart(const CL_Vec3f& min, const CL_Vec3f& max) {
    startVelocity_.set(max, min);
}

void MotionModelStartEndVelocity::setVelocityAndAccelerationEnd(float min, float max) {
    endVelocity_.set(startVelocity_.getT0() * min, startVelocity_.getT0() * max);
}

void MotionModelStartEndVelocity::get(const CL_Vec3f& emitterLocation, const CL_Vec3f& particleLocation, CL_Vec3f& outParam1, CL_Vec3f& outParam2) const {
    outParam1 = startVelocity_.get();
    outParam2 = endVelocity_.get();
}


void MotionModelAwayFromEmitter::setAccelerationStart(float min, float max) {
    startAcceleration_.set(min, max);
}

void MotionModelAwayFromEmitter::setAccelerationEnd(float min, float max) {
    endAcceleration_.set(min, max);
}

void MotionModelAwayFromEmitter::get(const CL_Vec3f& emitterLocation, const CL_Vec3f& particleLocation, CL_Vec3f& outParam1, CL_Vec3f& outParam2) const {
    CL_Vec3f direction = particleLocation - emitterLocation;
    direction.normalize();

    outParam1 = direction * startAcceleration_.get();
    outParam2 = direction * endAcceleration_.get();
}

}
}
}
