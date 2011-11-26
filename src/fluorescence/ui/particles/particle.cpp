
#include "particle.hpp"

namespace fluo {
namespace ui {
namespace particles {

Particle::Particle() : colorStart_(0, 0, 0, 0), colorEnd_(0, 0, 0, 0) {
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

//void Particle::initialize(const CL_Vec3f& startPosition, float creationTime) {
    //colorStart_ = CL_Vec4f(1, 0, 0, 0.8);
    //colorEnd_ = CL_Vec4f(0, 0, 1, 0.2);

    //startPosition_ = startPosition;

    //velocityStart_ = randMinMax(CL_Vec3f(2, -1, 0), CL_Vec3f(3, 1, 0));
    //velocityEnd_ = randMinMax(CL_Vec3f(1, -1, 0), CL_Vec3f(2, 1, 0));

    //creationTime_ = creationTime;
    //expireTime_ = creationTime_ + randMinMax(5.f, 6.f);
//}

}
}
}
