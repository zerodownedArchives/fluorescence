
#include "emitter.hpp"

#include <algorithm>

#include "startpositionprovider.hpp"
#include "motionmodel.hpp"

namespace fluo {
namespace ui {
namespace particles {
    
Emitter::Emitter() : 
        age_(0),
        emittedFractionStore_(0.99999f) // to emit at least one child at the start
        {
}

void Emitter::update(float elapsedSeconds) {
    //float elapsedSeconds = ((float)elapsedMillis / 1000.f);

    // update own properties
    age_ += elapsedSeconds;
    float expireAge = (lifetimes_[1u] - lifetimes_[0u]);
    normalizedAge_ = (std::min)(1.0f, age_ / expireAge); // limit to 1.0
    CL_Vec3f positionDelta = velocityStart_ * normalizedAge_ +
            (velocityEnd_ - velocityStart_) * normalizedAge_ * normalizedAge_ / 2.0;
    position_ = startPosition_ + positionDelta * expireAge;

    emitPerSecond_.setNormalizedAge(normalizedAge_);
    emittedStartPositionProvider_->setNormalizedAge(normalizedAge_);
    emittedMotionModel_->setNormalizedAge(normalizedAge_);
    emittedLifetimeMin_.setNormalizedAge(normalizedAge_);
    emittedLifetimeMax_.setNormalizedAge(normalizedAge_);


    float newCountF  = elapsedSeconds * emitPerSecond_ + emittedFractionStore_;
    unsigned int newCount = newCountF;

    if (newCount > 0) {
        updateSet(newCount, elapsedSeconds);
    }

    emittedFractionStore_ = newCountF - newCount;
}

bool Emitter::isEmitting() const {
    return lifetimes_[0u] + age_ <= lifetimes_[1u];
}

}
}
}
