
#include "emitteremitter.hpp"

namespace fluo {
namespace ui {
namespace particles {

unsigned int EmitterEmitter::emittedCount() const {
    return emitters_.size();
}

void EmitterEmitter::updateSet(unsigned int newCount, float elapsedSeconds) {
    for (boost::shared_ptr<Emitter>& em : emitters_) {
        if (!em->isExpired()) {
            em->update(elapsedSeconds);
        }
    }

    // TODO: create new ones
}

void EmitterEmitter::render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader) {
    for (boost::shared_ptr<Emitter>& em : emitters_) {
        if (!em->isExpired()) {
            em->render(gc, shader);
        }
    }
}

bool EmitterEmitter::isExpired() const {
    return this->Emittable::isExpired(age_);
}

}
}
}
