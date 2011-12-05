
#include "motionmodel.hpp"

#include <misc/random.hpp>

namespace fluo {
namespace ui {
namespace particles {

MotionModel::MotionModel(unsigned int id) : id_(id) {
}

unsigned int MotionModel::getId() const {
    return id_;
}


MotionModelStatic::MotionModelStatic() : MotionModel(TYPE_STATIC) {
}

void MotionModelStatic::setNormalizedAge(float age) {
    // do nothing
}

void MotionModelStatic::get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const {
    param1 = CL_Vec3f(0, 0, 0);
    param2 = CL_Vec3f(0, 0, 0);
}


MotionModelStartEndVelocity::MotionModelStartEndVelocity() :
        MotionModel(MotionModel::TYPE_START_END_VELOCITY) {
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


MotionModelAwayFromEmitter::MotionModelAwayFromEmitter() :
        MotionModel(MotionModel::TYPE_AWAY_FROM_EMITTER) {
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
