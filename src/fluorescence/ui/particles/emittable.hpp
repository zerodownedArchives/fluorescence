#ifndef FLUO_UI_PARTICLES_EMITTABLE_HPP
#define FLUO_UI_PARTICLES_EMITTABLE_HPP

#include <ClanLib/Core/Math/vec3.h>

namespace fluo {
namespace ui {
namespace particles {

class Emitter;

class Emittable {

friend class Emitter;

public:
    Emittable();

    Emittable(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime);
    void reset(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime);

    bool isExpired(float emitterAge) const;

    static float randMinMax(float min, float max);
    static unsigned int randMinMax(unsigned int min, unsigned int max);
    static CL_Vec3f randMinMax(const CL_Vec3f& min, const CL_Vec3f& max);
    static CL_Vec4f randMinMax(const CL_Vec4f& min, const CL_Vec4f& max);


protected:
    CL_Vec3f startPosition_;

    CL_Vec3f velocityStart_;
    CL_Vec3f velocityEnd_;

    CL_Vec2f lifetimes_; // 0 - creation, 1 - expiration
};

}
}
}

#endif
