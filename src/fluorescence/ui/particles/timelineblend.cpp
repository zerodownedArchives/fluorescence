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
 
#include "timelineblend.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace particles {
    
TimelineBlend::TimelineBlend(const ParticleEmitterState& state1, const ParticleEmitterState& state2, float duration) : 
        state1_(state1), state2_(state2), duration_(duration) {
}
    
void TimelineBlend::activate() {
    LOG_DEBUG << "timeline blend activated, from=" << state1_.name_ << " to=" << state2_.name_ << std::endl;
    timeExpired_ = 0;
}

float TimelineBlend::step(float elapsedSeconds) {
    float realStep = (std::min)(duration_ - timeExpired_, elapsedSeconds);
    timeExpired_ += realStep;
    normalizedAge_ = (std::min)(1.0f, timeExpired_ / duration_);
    return elapsedSeconds - realStep;
}

float TimelineBlend::numberOfNewParticles(float elapsedSeconds) const {
    float realStep = (std::min)(duration_ - timeExpired_, elapsedSeconds);
    // interpolate emit frequency between the two states
    float freq = (state1_.emitFrequency_ * (1 - normalizedAge_)) + (state2_.emitFrequency_ * normalizedAge_);
    float numNew = freq * realStep;
    return numNew;
}

void TimelineBlend::initParticle(Particle& particle, const CL_Vec3f& emitterLocation, float emitterAge) const {
    if (Random::random01() > normalizedAge_) {
        state1_.initParticle(particle, emitterLocation, emitterAge);
    } else {
        state2_.initParticle(particle, emitterLocation, emitterAge);
    }
}

CL_Vec3f TimelineBlend::getEmitterLocationOffset() const {
    // interpolate position between the two states
    return (state1_.emitterLocationOffset_ * (1 - normalizedAge_)) + (state2_.emitterLocationOffset_ * normalizedAge_);
}

float TimelineBlend::getMaxParticleLifetime() const {
    return (std::max)(state1_.emittedLifetime_.getMax(), state2_.emittedLifetime_.getMax());
}

}
}
}
