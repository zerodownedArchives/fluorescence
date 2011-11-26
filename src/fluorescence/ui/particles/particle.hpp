#ifndef FLUO_UI_PARTICLES_PARTICLE_HPP
#define FLUO_UI_PARTICLES_PARTICLE_HPP

#include <ClanLib/Core/Math/vec4.h>

#include "emittable.hpp"

namespace fluo {
namespace ui {
namespace particles {

class ParticleEmitter;

class Particle : public Emittable {

friend class ParticleEmitter;

public:
    Particle();

    Particle(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, const CL_Vec4f colStart, const CL_Vec4f& colEnd);
    void reset(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, const CL_Vec4f colStart, const CL_Vec4f& colEnd);


protected:
    CL_Vec4f colorStart_;
    CL_Vec4f colorEnd_;
};

}
}
}

#endif
