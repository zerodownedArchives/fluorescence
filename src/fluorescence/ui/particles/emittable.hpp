#ifndef FLUO_UI_PARTICLES_EMITTABLE_HPP
#define FLUO_UI_PARTICLES_EMITTABLE_HPP

#include <ClanLib/Core/Math/vec3.h>

namespace fluo {
namespace ui {
namespace particles {

class Emittable {
public:
    CL_Vec3f startPosition_;

    CL_Vec3f velocityStart_;
    CL_Vec3f velocityEnd_;

    float creationTime_;
    float lifetimeSeconds_;


    static float randMinMax(float min, float max);
    static unsigned int randMinMax(unsigned int min, unsigned int max);
    static CL_Vec3f randMinMax(const CL_Vec3f& min, const CL_Vec3f& max);
    static CL_Vec4f randMinMax(const CL_Vec4f& min, const CL_Vec4f& max);

};

}
}
}

#endif
