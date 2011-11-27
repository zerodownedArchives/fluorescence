
#include "emitter.hpp"

#include <algorithm>

#include "startpositionprovider.hpp"

namespace fluo {
namespace ui {
namespace particles {


Emitter::Emitter(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, unsigned int maxCount,
        float emitPerSec, bool emittedMoveWithEmitter, const boost::shared_ptr<StartPositionProvider>& startPosProvider) :
    Emittable(startPos, velStart, velEnd, creationTime, expireTime),
    emittedMaxCount_(maxCount),
    emitPerSecond_(emitPerSec),
    position_(startPosition_),
    age_(0),
    emittedFractionStore_(0.99999f), // to emit at least one child at the start
    emittedMoveWithEmitter_(emittedMoveWithEmitter),
    startPositionProvider_(startPosProvider)
    {

}

void Emitter::reset(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, unsigned int maxCount,
        float emitPerSec, bool emittedMoveWithEmitter, const boost::shared_ptr<StartPositionProvider>& startPosProvider) {

    Emittable::reset(startPos, velStart, velEnd, creationTime, expireTime);
    emittedMaxCount_ = maxCount;
    emitPerSecond_ = emitPerSec;
    position_ = startPosition_;
    emittedMoveWithEmitter = emittedMoveWithEmitter;
    age_ = 0;
    emittedFractionStore_ = 0.99999f; // to emit at least one child at the start
    startPositionProvider_ = startPosProvider;
}

void Emitter::update(float elapsedSeconds) {
    //float elapsedSeconds = ((float)elapsedMillis / 1000.f);

    // update own properties
    age_ += elapsedSeconds;
    float expireAge = (lifetimes_[1u] - lifetimes_[0u]);
    normalizedAge_ = age_ / expireAge;
    CL_Vec3f positionDelta = velocityStart_ * normalizedAge_ +
            (velocityEnd_ - velocityStart_) * normalizedAge_ * normalizedAge_ / 2.0;
    position_ = startPosition_ + positionDelta * expireAge;
    startPositionProvider_->setNormalizedAge(normalizedAge_);


    float newCountF  = elapsedSeconds * emitPerSecond_ + emittedFractionStore_;
    unsigned int newCount = newCountF;

    if (newCount > 0) {
        updateSet(newCount, elapsedSeconds);
    }

    emittedFractionStore_ = newCountF - newCount;
}

}
}
}
