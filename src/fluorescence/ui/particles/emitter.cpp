
#include "emitter.hpp"

#include <algorithm>

namespace fluo {
namespace ui {
namespace particles {


Emitter::Emitter(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, unsigned int maxCount,
        float emitPerSec, bool emittedMoveWithEmitter) :
    Emittable(startPos, velStart, velEnd, creationTime, expireTime),
    emittedMaxCount_(maxCount),
    emitPerSecond_(emitPerSec),
    position_(startPosition_),
    age_(0),
    emittedFractionStore_(0.99999f), // to emit at least one child at the start
    emittedMoveWithEmitter_(emittedMoveWithEmitter)
    {

}

void Emitter::reset(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, unsigned int maxCount,
        float emitPerSec, bool emittedMoveWithEmitter) {

    Emittable::reset(startPos, velStart, velEnd, creationTime, expireTime);
    emittedMaxCount_ = maxCount;
    emitPerSecond_ = emitPerSec;
    position_ = startPosition_;
    emittedMoveWithEmitter = emittedMoveWithEmitter;
    age_ = 0;
    emittedFractionStore_ = 0.99999f; // to emit at least one child at the start
}

void Emitter::update(float elapsedSeconds) {
    //float elapsedSeconds = ((float)elapsedMillis / 1000.f);

    // TODO: update own properties (position etc)

    age_ += elapsedSeconds;


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
