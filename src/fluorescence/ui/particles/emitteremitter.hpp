#ifndef FLUO_UI_PARTICLES_EMITTEREMITTER_HPP
#define FLUO_UI_PARTICLES_EMITTEREMITTER_HPP

#include "emitter.hpp"

#include <vector>
#include <boost/shared_ptr.hpp>

namespace fluo {
namespace ui {
namespace particles {

class EmitterEmitter : public Emitter {
public:
    virtual unsigned int emittedCount() const;
    virtual void emit(unsigned int count);
    virtual void render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader);

private:
    std::vector<boost::shared_ptr<Emitter> > emitters_;

    virtual void onUpdate(unsigned int elapsedMillis);
};

}
}
}

#endif
