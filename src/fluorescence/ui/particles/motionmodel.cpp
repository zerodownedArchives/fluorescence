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

#include <misc/random.hpp>

namespace fluo {
namespace ui {
namespace particles {

void MotionModelStatic::setNormalizedAge(float age) {
    // do nothing
}

void MotionModelStatic::get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const {
    param1 = CL_Vec3f(0, 0, 0);
    param2 = CL_Vec3f(0, 0, 0);
}


MotionModelStartEndVelocity::MotionModelStartEndVelocity() {
}

void MotionModelStartEndVelocity::setVelocitiesT0(const CL_Vec3f& startMin, const CL_Vec3f& startMax, const CL_Vec3f& endMin, const CL_Vec3f& endMax) {
    startVelocityMin_.setT0(startMin);
    startVelocityMax_.setT0(startMax);
    endVelocityMin_.setT0(endMin);
    endVelocityMax_.setT0(endMax);
}

void MotionModelStartEndVelocity::setVelocitiesT1(const CL_Vec3f& startMin, const CL_Vec3f& startMax, const CL_Vec3f& endMin, const CL_Vec3f& endMax) {
    startVelocityMin_.setT1(startMin);
    startVelocityMax_.setT1(startMax);
    endVelocityMin_.setT1(endMin);
    endVelocityMax_.setT1(endMax);
}

void MotionModelStartEndVelocity::setVelocityAndAccelerationT0(const CL_Vec3f& startMin, const CL_Vec3f& startMax, float accelMin, float accelMax) {
    setVelocitiesT0(startMin, startMax, startMin * accelMin, startMax * accelMax);
}

void MotionModelStartEndVelocity::setVelocityAndAccelerationT1(const CL_Vec3f& startMin, const CL_Vec3f& startMax, float accelMin, float accelMax) {
    setVelocitiesT1(startMin, startMax, startMin * accelMin, startMax * accelMax);
}

void MotionModelStartEndVelocity::setNormalizedAge(float age) {
    startVelocityMin_.setNormalizedAge(age);
    startVelocityMax_.setNormalizedAge(age);
    endVelocityMin_.setNormalizedAge(age);
    endVelocityMax_.setNormalizedAge(age);
}

void MotionModelStartEndVelocity::get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const {
    param1 = Random::randomMinMax(startVelocityMin_, startVelocityMax_);
    param2 = Random::randomMinMax(endVelocityMin_, endVelocityMax_);
}


MotionModelAwayFromEmitter::MotionModelAwayFromEmitter() {
}

void MotionModelAwayFromEmitter::setAccelerationT0(float startMin, float startMax, float endMin, float endMax) {
    startAccelerationMin_.setT0(startMin);
    startAccelerationMax_.setT0(startMax);
    endAccelerationMin_.setT0(endMin);
    endAccelerationMax_.setT0(endMax);
}

void MotionModelAwayFromEmitter::setAccelerationT1(float startMin, float startMax, float endMin, float endMax) {
    startAccelerationMin_.setT1(startMin);
    startAccelerationMax_.setT1(startMax);
    endAccelerationMin_.setT1(endMin);
    endAccelerationMax_.setT1(endMax);
}

void MotionModelAwayFromEmitter::setNormalizedAge(float age) {
    startAccelerationMin_.setNormalizedAge(age);
    startAccelerationMax_.setNormalizedAge(age);
    endAccelerationMin_.setNormalizedAge(age);
    endAccelerationMax_.setNormalizedAge(age);
}

void MotionModelAwayFromEmitter::get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const {
    CL_Vec3f direction = particlePosition - emitterPosition;
    direction.normalize();

    param1 = direction * Random::randomMinMax(startAccelerationMin_, startAccelerationMax_);
    param2 = direction * Random::randomMinMax(endAccelerationMin_, endAccelerationMax_);
}

}
}
}
