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


void MotionModelStartEndVelocity::setVelocities(const CL_Vec3f& startMin, const CL_Vec3f& startMax, const CL_Vec3f& endMin, const CL_Vec3f& endMax) {
    startVelocity_.set(startMin, startMax);
    endVelocity_.set(endMin, endMax);
}

void MotionModelStartEndVelocity::setVelocityAndAcceleration(const CL_Vec3f& startMin, const CL_Vec3f& startMax, float accelMin, float accelMax) {
    setVelocities(startMin, startMax, startMin * accelMin, startMax * accelMax);
}

void MotionModelStartEndVelocity::get(const CL_Vec3f& emitterLocation, const CL_Vec3f& particleLocation, CL_Vec3f& outParam1, CL_Vec3f& outParam2) const {
    outParam1 = startVelocity_.get();
    outParam2 = endVelocity_.get();
}


void MotionModelAwayFromEmitter::setAcceleration(float startMin, float startMax, float endMin, float endMax) {
    startAcceleration_.set(startMin, startMax);
    endAcceleration_.set(endMin, endMax);
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
