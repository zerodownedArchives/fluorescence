
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
