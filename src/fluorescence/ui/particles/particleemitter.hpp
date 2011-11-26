#ifndef FLUO_UI_PARTICLES_PARTICLEEMITTER_HPP
#define FLUO_UI_PARTICLES_PARTICLEEMITTER_HPP

#include "emitter.hpp"
#include "particle.hpp"

namespace fluo {
namespace ui {
namespace particles {

class ParticleEmitter : public Emitter {
public:
    ParticleEmitter(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, unsigned int maxCount,
            float emitPerSec, bool emittedMoveWithEmitter);
    void reset(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, unsigned int maxCount,
            float emitPerSec, bool emittedMoveWithEmitter);

    ~ParticleEmitter();

    virtual unsigned int emittedCount() const;
    virtual void updateSet(unsigned int newCount, float elapsedSeconds);
    virtual void render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader);

private:
    unsigned int particleCount_;
    Particle* particles_;

    void initParticle(unsigned int index);


    // parameters for the emitted particles
    CL_Vec4f emittedColorStartMin_;
    CL_Vec4f emittedColorStartMax_;
    CL_Vec4f emittedColorEndMin_;
    CL_Vec4f emittedColorEndMax_;
};

}
}
}

#endif
