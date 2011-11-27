
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

CL_Vec3f MotionModel::interpolate(const CL_Vec3f& a, const CL_Vec3f& b, float factor) {
    return CL_Vec3f(
        a.x + (b.x - a.x) * factor,
        a.y + (b.y - a.y) * factor,
        a.z + (b.z - a.z) * factor
    );
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
    startVelocityMinCur_ = interpolate(startVelocityMinStart_, startVelocityMinEnd_, age);
    startVelocityMaxCur_ = interpolate(startVelocityMaxStart_, startVelocityMaxEnd_, age);
    endVelocityMinCur_ = interpolate(endVelocityMinStart_, endVelocityMinEnd_, age);
    endVelocityMaxCur_ = interpolate(endVelocityMaxStart_, endVelocityMaxEnd_, age);
}

CL_Vec3f MotionModelStartEndVelocity::getParam1(const CL_Vec3f& emitterPosition) const {
    return Random::randomMinMax(startVelocityMinCur_, startVelocityMaxCur_);
}

CL_Vec3f MotionModelStartEndVelocity::getParam2(const CL_Vec3f& emitterPosition) const {
    return Random::randomMinMax(endVelocityMinCur_, endVelocityMaxCur_);
}


}
}
}
