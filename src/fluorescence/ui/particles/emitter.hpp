#ifndef FLUO_UI_PARTICLES_EMITTER_HPP
#define FLUO_UI_PARTICLES_EMITTER_HPP

#include "emittable.hpp"

#include <boost/shared_ptr.hpp>
#include <ClanLib/Display/Render/graphic_context.h>
class CL_ProgramObject;

#include <misc/interpolation.hpp>

namespace fluo {
namespace ui {
namespace particles {

class StartPositionProvider;
class MotionModel;

class Emitter : public Emittable {
public:
    Emitter();

    // updateSet is used to propagate the change internally (e.g. to children)
    virtual void update(float elapsedSeconds);

    virtual unsigned int emittedCount() const = 0;

    virtual void render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader) = 0;
    
    virtual bool isExpired() const = 0;
    bool isEmitting() const;


protected:
    virtual void updateSet(unsigned int newCount, float elapsedSeconds) = 0;

    unsigned int emittedStartCount_;
    unsigned int emittedMaxCount_;
    InterpolatedValue<float> emitPerSecond_;

    CL_Vec3f position_;

    float age_;
    float normalizedAge_;
    float emittedFractionStore_;


    // parameters for the emitted objects
    InterpolatedValue<float> emittedLifetimeMin_;
    InterpolatedValue<float> emittedLifetimeMax_;

    bool emittedMoveWithEmitter_;

    boost::shared_ptr<StartPositionProvider> emittedStartPositionProvider_;
    boost::shared_ptr<MotionModel> emittedMotionModel_;
};

}
}
}

#endif
