/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



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
