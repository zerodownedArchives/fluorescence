
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

float Emittable::randMinMax(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

unsigned int Emittable::randMinMax(unsigned int min, unsigned int max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

CL_Vec3f Emittable::randMinMax(const CL_Vec3f& min, const CL_Vec3f& max) {
    CL_Vec3f ret;
    ret.x = randMinMax(min.x, max.x);
    ret.y = randMinMax(min.y, max.y);
    ret.z = randMinMax(min.z, max.z);
    return ret;
}

CL_Vec4f Emittable::randMinMax(const CL_Vec4f& min, const CL_Vec4f& max) {
    CL_Vec4f ret;
    ret.x = randMinMax(min.x, max.x);
    ret.y = randMinMax(min.y, max.y);
    ret.z = randMinMax(min.z, max.z);
    ret.w = randMinMax(min.w, max.w);
    return ret;
}

bool Emittable::isExpired(float emitterAge) const {
    return emitterAge > lifetimes_[1u];
}

}
}
}
