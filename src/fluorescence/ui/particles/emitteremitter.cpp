
#include "emitteremitter.hpp"

namespace fluo {
namespace ui {
namespace particles {

unsigned int EmitterEmitter::emittedCount() const {
    return emitters_.size();
}

void EmitterEmitter::emit(unsigned int count) {
}

void EmitterEmitter::onUpdate(unsigned int elapsedMillis) {
    std::vector<boost::shared_ptr<Emitter> >::iterator iter = emitters_.begin();
    std::vector<boost::shared_ptr<Emitter> >::iterator end = emitters_.end();

    for (; iter != end; ++iter) {
        if (!(*iter)->isExpired()) {
            (*iter)->onUpdate(elapsedMillis);
        }
    }
}

void EmitterEmitter::render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader) {
    std::vector<boost::shared_ptr<Emitter> >::iterator iter = emitters_.begin();
    std::vector<boost::shared_ptr<Emitter> >::iterator end = emitters_.end();

    for (; iter != end; ++iter) {
        if (!(*iter)->isExpired()) {
            (*iter)->render(gc);
        }
    }
}

}
}
}
