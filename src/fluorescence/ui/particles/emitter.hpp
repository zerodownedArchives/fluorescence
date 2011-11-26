#ifndef FLUO_UI_PARTICLES_EMITTER_HPP
#define FLUO_UI_PARTICLES_EMITTER_HPP

#include "emittable.hpp"

#include <boost/shared_ptr.hpp>
#include <ClanLib/Display/Render/graphic_context.h>
class CL_ProgramObject;

namespace fluo {
namespace ui {
namespace particles {

class Emitter : public Emittable {
public:
    Emitter(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, unsigned int maxCount,
            float emitPerSec, bool emittedMoveWithEmitter);
    void reset(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, unsigned int maxCount,
            float emitPerSec, bool emittedMoveWithEmitter);

    // updateSet is used to propagate the change internally (e.g. to children)
    virtual void update(float elapsedSeconds);

    virtual unsigned int emittedCount() const = 0;

    virtual void render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader) = 0;


protected:
    virtual void updateSet(unsigned int newCount, float elapsedSeconds) = 0;

    unsigned int emittedMaxCount_;
    float emitPerSecond_;

    CL_Vec3f position_;

    float age_;
    float emittedFractionStore_;


    // parameters for the emitted objects
    CL_Vec3f emittedVelocityStartMin_;
    CL_Vec3f emittedVelocityStartMax_;
    CL_Vec3f emittedVelocityEndMin_;
    CL_Vec3f emittedVelocityEndMax_;
    float emittedLifetimeMin_;
    float emittedLifetimeMax_;

    bool emittedMoveWithEmitter_;
};

}
}
}

#endif
