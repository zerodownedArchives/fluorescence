#ifndef FLUO_INTERPOLATION_HPP
#define FLUO_INTERPOLATION_HPP

#include <ClanLib/Core/Math/vec3.h>

namespace fluo {

class Interpolation {
public:
    static float linear(float a, float b, float factor);
    static CL_Vec3f linear(const CL_Vec3f& a, const CL_Vec3f& b, float factor);
};

}
#endif
