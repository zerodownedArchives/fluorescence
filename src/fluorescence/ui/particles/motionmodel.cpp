
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
    startVelocityMinT0_ = startMin;
    startVelocityMaxT0_ = startMax;
    endVelocityMinT0_ = endMin;
    endVelocityMaxT0_ = endMax;
}

void MotionModelStartEndVelocity::setVelocitiesT1(const CL_Vec3f& startMin, const CL_Vec3f& startMax, const CL_Vec3f& endMin, const CL_Vec3f& endMax) {
    startVelocityMinT1_ = startMin;
    startVelocityMaxT1_ = startMax;
    endVelocityMinT1_ = endMin;
    endVelocityMaxT1_ = endMax;
}

void MotionModelStartEndVelocity::setVelocityAndAccelerationT0(const CL_Vec3f& startMin, const CL_Vec3f& startMax, float accelMin, float accelMax) {
    setVelocitiesT0(startMin, startMax, startMin * accelMin, startMax * accelMax);
}

void MotionModelStartEndVelocity::setVelocityAndAccelerationT1(const CL_Vec3f& startMin, const CL_Vec3f& startMax, float accelMin, float accelMax) {
    setVelocitiesT1(startMin, startMax, startMin * accelMin, startMax * accelMax);
}

void MotionModelStartEndVelocity::setNormalizedAge(float age) {
    startVelocityMinTx_ = Interpolation::linear(startVelocityMinT0_, startVelocityMinT1_, age);
    startVelocityMaxTx_ = Interpolation::linear(startVelocityMaxT0_, startVelocityMaxT1_, age);
    endVelocityMinTx_ = Interpolation::linear(endVelocityMinT0_, endVelocityMinT1_, age);
    endVelocityMaxTx_ = Interpolation::linear(endVelocityMaxT0_, endVelocityMaxT1_, age);
}

void MotionModelStartEndVelocity::get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const {
    param1 = Random::randomMinMax(startVelocityMinTx_, startVelocityMaxTx_);
    param2 = Random::randomMinMax(endVelocityMinTx_, endVelocityMaxTx_);
}


MotionModelAwayFromEmitter::MotionModelAwayFromEmitter() :
        MotionModel(MotionModel::TYPE_AWAY_FROM_EMITTER) {
}

void MotionModelAwayFromEmitter::setAccelerationT0(float startMin, float startMax, float endMin, float endMax) {
    startAccelerationMinT0_ = startMin;
    startAccelerationMaxT0_ = startMax;
    endAccelerationMinT0_ = endMin;
    endAccelerationMaxT0_ = endMax;
}

void MotionModelAwayFromEmitter::setAccelerationT1(float startMin, float startMax, float endMin, float endMax) {
    startAccelerationMinT1_ = startMin;
    startAccelerationMaxT1_ = startMax;
    endAccelerationMinT1_ = endMin;
    endAccelerationMaxT1_ = endMax;
}

void MotionModelAwayFromEmitter::setNormalizedAge(float age) {
    startAccelerationMinTx_ = Interpolation::linear(startAccelerationMinT0_, startAccelerationMinT1_, age);
    startAccelerationMaxTx_ = Interpolation::linear(startAccelerationMaxT0_, startAccelerationMaxT1_, age);
    endAccelerationMinTx_ = Interpolation::linear(endAccelerationMinT0_, endAccelerationMinT1_, age);
    endAccelerationMaxTx_ = Interpolation::linear(endAccelerationMaxT0_, endAccelerationMaxT1_, age);
}

void MotionModelAwayFromEmitter::get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const {
    CL_Vec3f direction = particlePosition - emitterPosition;
    direction.normalize();

    param1 = direction * Random::randomMinMax(startAccelerationMinTx_, startAccelerationMaxTx_);
    param2 = direction * Random::randomMinMax(endAccelerationMinTx_, endAccelerationMaxTx_);
}

}
}
}
