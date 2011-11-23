#ifndef FLUO_UI_PARTICLES_EMITTER_HPP
#define FLUO_UI_PARTICLES_EMITTER_HPP

#include "emittable.hpp"

#include "ClanLib/Display/Render/graphic_context.h"

namespace fluo {
namespace ui {
namespace particles {

class Emitter : public Emittable {
public:
    unsigned int emittedMaxCount_;
    float emitPerSecond_;
    float elapsedSecondsStore_;
    float age_;

    bool emittedMoveWithEmitter_;

    CL_Vec3f position_;

    // overloaded from ingameobject
    virtual void update(unsigned int elapsedMillis);

    // to pass on to children
    virtual void onUpdate(unsigned int elapsedMillis) = 0;

    virtual unsigned int emittedCount() const = 0;

    bool isExpired() const;

    virtual void render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader) = 0;


protected:
    virtual void emit(unsigned int count) = 0;
};

}
}
}

#endif
