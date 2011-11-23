
#include "particleemitter.hpp"

namespace fluo {
namespace ui {
namespace particles {

unsigned int ParticleEmitter::emittedCount() const {
    return particles_.size();
}

void ParticleEmitter::emit(unsigned int count) {
    unsigned int curCnt = emittedCount();
    unsigned int targCnt = (std::min)(curCnt + count, emittedMaxCount_);

    for (; curCnt < targCnt; ++curCnt) {
        particles_[curCnt].initialize(position_, age_);
    }
}

void ParticleEmitter::onUpdate(unsigned int elapsedMillis) {
}

void ParticleEmitter::render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader) {
    // set shader uniform variables
    shader->set_uniform1b("EmittedMoveWithEmitter", false);
    shader->set_uniform1f("CurrentTime", age_);

    // collect particle data in primarray
    CL_PrimitivesArray primarray(gc);

    primarray.set_attributes(0, batchPositions_);
    primarray.set_attributes(1, batchTexCoords_);

    // call to draw
    gc.draw_primitives(cl_points, emittedCount(), primarray);
}

void ParticleEmitter::setCapacity() {
    particles_.reserve(emittedMaxCount_);
}

}
}
}
