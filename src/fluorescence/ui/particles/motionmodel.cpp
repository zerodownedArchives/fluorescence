
#include "motionmodel.hpp"

#include <misc/random.hpp>
#include <misc/interpolation.hpp>

namespace fluo {
namespace ui {
namespace particles {

MotionModel::MotionModel(unsigned int id) : id_(id) {
}

unsigned int MotionModel::getId() const {
    return id_;
}


MotionModelStartEndVelocity::MotionModelStartEndVelocity() :
        MotionModel(MotionModel::TYPE_START_END_VELOCITY) {
}

void MotionModelStartEndVelocity::setStartVelocities(const CL_Vec3f& startMin, const CL_Vec3f& startMax, const CL_Vec3f& endMin, const CL_Vec3f& endMax) {
    startVelocityMinStart_ = startMin;
    startVelocityMaxStart_ = startMax;
    endVelocityMinStart_ = endMin;
    endVelocityMaxStart_ = endMax;
}

void MotionModelStartEndVelocity::setEndVelocities(const CL_Vec3f& startMin, const CL_Vec3f& startMax, const CL_Vec3f& endMin, const CL_Vec3f& endMax) {
    startVelocityMinEnd_ = startMin;
    startVelocityMaxEnd_ = startMax;
    endVelocityMinEnd_ = endMin;
    endVelocityMaxEnd_ = endMax;
}

void MotionModelStartEndVelocity::setNormalizedAge(float age) {
    startVelocityMinCur_ = Interpolation::linear(startVelocityMinStart_, startVelocityMinEnd_, age);
    startVelocityMaxCur_ = Interpolation::linear(startVelocityMaxStart_, startVelocityMaxEnd_, age);
    endVelocityMinCur_ = Interpolation::linear(endVelocityMinStart_, endVelocityMinEnd_, age);
    endVelocityMaxCur_ = Interpolation::linear(endVelocityMaxStart_, endVelocityMaxEnd_, age);
}

void MotionModelStartEndVelocity::get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const {
    param1 = Random::randomMinMax(startVelocityMinCur_, startVelocityMaxCur_);
    param2 = Random::randomMinMax(endVelocityMinCur_, endVelocityMaxCur_);
}


MotionModelAwayFromEmitter::MotionModelAwayFromEmitter() :
        MotionModel(MotionModel::TYPE_AWAY_FROM_EMITTER) {
}

void MotionModelAwayFromEmitter::setStartAcceleration(float startMin, float startMax, float endMin, float endMax) {
    startAccelerationMinStart_ = startMin;
    startAccelerationMaxStart_ = startMax;
    endAccelerationMinStart_ = endMin;
    endAccelerationMaxStart_ = endMax;
}

void MotionModelAwayFromEmitter::setEndAcceleration(float startMin, float startMax, float endMin, float endMax) {
    startAccelerationMinEnd_ = startMin;
    startAccelerationMaxEnd_ = startMax;
    endAccelerationMinEnd_ = endMin;
    endAccelerationMaxEnd_ = endMax;
}

void MotionModelAwayFromEmitter::setNormalizedAge(float age) {
    startAccelerationMinCur_ = Interpolation::linear(startAccelerationMinStart_, startAccelerationMinEnd_, age);
    startAccelerationMaxCur_ = Interpolation::linear(startAccelerationMaxStart_, startAccelerationMaxEnd_, age);
    endAccelerationMinCur_ = Interpolation::linear(endAccelerationMinStart_, endAccelerationMinEnd_, age);
    endAccelerationMaxCur_ = Interpolation::linear(endAccelerationMaxStart_, endAccelerationMaxEnd_, age);
}

void MotionModelAwayFromEmitter::get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const {
    CL_Vec3f direction = particlePosition - emitterPosition;
    direction.normalize();

    param1 = direction * Random::randomMinMax(startAccelerationMinCur_, startAccelerationMaxCur_);
    param2 = direction * Random::randomMinMax(endAccelerationMinCur_, endAccelerationMaxCur_);
}


}
}
}
