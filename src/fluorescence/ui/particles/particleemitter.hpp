#ifndef FLUO_UI_PARTICLES_PARTICLEEMITTER_HPP
#define FLUO_UI_PARTICLES_PARTICLEEMITTER_HPP

#include "emitter.hpp"
#include "particle.hpp"

#include <vector>

namespace fluo {
namespace ui {
namespace particles {

class ParticleEmitter : public Emitter {
public:
    virtual unsigned int emittedCount() const;
    virtual void emit(unsigned int count);
    virtual void render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader);

    void setCapacity();

private:
    std::vector<Particle> particles_;

    virtual void onUpdate(unsigned int elapsedMillis);
};

}
}
}

#endif
