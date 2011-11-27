
#include "random.hpp"

#include <stdlib.h>

namespace fluo {

bool Random::randomBool() {
    return random01() < 0.5;
}

float Random::random01() {
    return ((float)rand() / RAND_MAX);
}

float Random::randomMinMax(float min, float max) {
    return min + random01() * (max - min);
}

unsigned int Random::randomMinMax(unsigned int min, unsigned int max) {
    return min + random01() * (max - min);
}

CL_Vec3f Random::randomMinMax(const CL_Vec3f& min, const CL_Vec3f& max) {
    CL_Vec3f ret;
    ret.x = randomMinMax(min.x, max.x);
    ret.y = randomMinMax(min.y, max.y);
    ret.z = randomMinMax(min.z, max.z);
    return ret;
}

CL_Vec4f Random::randomMinMax(const CL_Vec4f& min, const CL_Vec4f& max) {
    CL_Vec4f ret;
    ret.x = randomMinMax(min.x, max.x);
    ret.y = randomMinMax(min.y, max.y);
    ret.z = randomMinMax(min.z, max.z);
    ret.w = randomMinMax(min.w, max.w);
    return ret;
}

}
