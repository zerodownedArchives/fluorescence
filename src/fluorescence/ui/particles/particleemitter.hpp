#ifndef FLUO_UI_PARTICLES_PARTICLEEMITTER_HPP
#define FLUO_UI_PARTICLES_PARTICLEEMITTER_HPP

#include "emitter.hpp"
#include "particle.hpp"

namespace fluo {
namespace ui {

class Texture;
    
namespace particles {
    
class XmlLoader;

class ParticleEmitter : public Emitter {

friend class XmlLoader;

public:
    ParticleEmitter(unsigned int maxSize);

    ~ParticleEmitter();

    virtual unsigned int emittedCount() const;
    virtual void updateSet(unsigned int newCount, float elapsedSeconds);
    virtual void render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader);
    
    virtual bool isExpired() const;

private:
    unsigned int particleCount_;
    Particle* particles_;

    void initParticle(unsigned int index);

    // parameters for the emitted particles
    InterpolatedValue<CL_Vec4f> emittedColorStartMin_;
    InterpolatedValue<CL_Vec4f> emittedColorStartMax_;
    InterpolatedValue<CL_Vec4f> emittedColorEndMin_;
    InterpolatedValue<CL_Vec4f> emittedColorEndMax_;
    
    boost::shared_ptr<ui::Texture> emittedTexture_;
};

}
}
}

#endif
