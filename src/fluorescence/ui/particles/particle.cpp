
#include "particle.hpp"

namespace fluo {
namespace ui {
namespace particles {

void Particle::initialize(const CL_Vec3f& startPosition, float creationTime) {
    colorStart_ = CL_Vec4f(1, 0, 0, 0.8);
    colorEnd_ = CL_Vec4f(0, 0, 1, 0.2);

    startPosition_ = startPosition;

    velocityStart_ = randMinMax(CL_Vec3f(2, -1, 0), CL_Vec3f(3, 1, 0));
    velocityEnd_ = randMinMax(CL_Vec3f(1, -1, 0), CL_Vec3f(2, 1, 0));

    creationTime_ = creationTime;
    lifetimeSeconds_ = randMinMax(5.f, 6.f);
}

}
}
}
