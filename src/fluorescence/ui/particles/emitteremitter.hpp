#ifndef FLUO_UI_PARTICLES_EMITTEREMITTER_HPP
#define FLUO_UI_PARTICLES_EMITTEREMITTER_HPP

#include "emitter.hpp"

#include <list>
#include <boost/shared_ptr.hpp>

namespace fluo {
namespace ui {
namespace particles {

// does not work yet.
class EmitterEmitter : public Emitter {
public:
    virtual unsigned int emittedCount() const;
    virtual void updateSet(unsigned int newCount, float elapsedSeconds);
    virtual void render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader);

private:
    std::list<boost::shared_ptr<Emitter> > emitters_;
};

}
}
}

#endif
