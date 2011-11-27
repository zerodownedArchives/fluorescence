
#include "particle.hpp"

namespace fluo {
namespace ui {
namespace particles {

Particle::Particle() : colorStart_(0, 0, 0, 0), colorEnd_(0, 0, 0, 0), isRemoved_(false) {
}

Particle::Particle(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, const CL_Vec4f colStart, const CL_Vec4f& colEnd) :
        Emittable(startPos, velStart, velEnd, creationTime, expireTime),
        colorStart_(colStart), colorEnd_(colEnd) {
}

void Particle::reset(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, const CL_Vec4f colStart, const CL_Vec4f& colEnd) {
    Emittable::reset(startPos, velStart, velEnd, creationTime, expireTime);
    colorStart_ = colStart;
    colorEnd_ = colEnd;
}

bool Particle::isRemoved() const {
    return isRemoved_;
}

void Particle::remove() {
    isRemoved_ = true;
    startPosition_.z = 10.0;
}

}
}
}
