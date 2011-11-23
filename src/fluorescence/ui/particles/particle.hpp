#ifndef FLUO_UI_PARTICLES_PARTICLE_HPP
#define FLUO_UI_PARTICLES_PARTICLE_HPP

#include <ClanLib/Core/Math/vec4.h>

#include "emittable.hpp"

namespace fluo {
namespace ui {
namespace particles {

class Particle : public Emittable {
public:
    CL_Vec4f colorStart_;
    CL_Vec4f colorEnd_;

    void initialize(const CL_Vec3f& startPosition, float creationTime);
};

}
}
}

#endif
