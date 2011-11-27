#ifndef FLUO_RANDOM_HPP
#define FLUO_RANDOM_HPP

#include <ClanLib/Core/Math/vec3.h>
#include <ClanLib/Core/Math/vec4.h>

namespace fluo {

class Random {
public:
    static bool randomBool();
    static float random01();
    static float randomMinMax(float min, float max);
    static unsigned int randomMinMax(unsigned int min, unsigned int max);
    static CL_Vec3f randomMinMax(const CL_Vec3f& min, const CL_Vec3f& max);
    static CL_Vec4f randomMinMax(const CL_Vec4f& min, const CL_Vec4f& max);
};

}

#endif
