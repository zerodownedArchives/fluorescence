
#include "emitteremitter.hpp"

namespace fluo {
namespace ui {
namespace particles {

unsigned int EmitterEmitter::emittedCount() const {
    return emitters_.size();
}

void EmitterEmitter::updateSet(unsigned int newCount, float elapsedSeconds) {
    std::list<boost::shared_ptr<Emitter> >::iterator iter = emitters_.begin();
    std::list<boost::shared_ptr<Emitter> >::iterator end = emitters_.end();

    for (; iter != end; ++iter) {
        if (!(*iter)->isExpired()) {
            (*iter)->update(elapsedSeconds);
        }
    }

    // TODO: create new ones
}

void EmitterEmitter::render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader) {
    std::list<boost::shared_ptr<Emitter> >::iterator iter = emitters_.begin();
    std::list<boost::shared_ptr<Emitter> >::iterator end = emitters_.end();

    for (; iter != end; ++iter) {
        if (!(*iter)->isExpired()) {
            (*iter)->render(gc, shader);
        }
    }
}

bool EmitterEmitter::isExpired() const {
    return this->Emittable::isExpired(age_);
}

}
}
}
