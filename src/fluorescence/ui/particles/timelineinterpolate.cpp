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
 
#include "timelineinterpolate.hpp"

#include <misc/log.hpp>
#include <misc/interpolation.hpp>

#include "particle.hpp"

namespace fluo {
namespace ui {
namespace particles {
    
TimelineInterpolate::TimelineInterpolate(const ParticleEmitterState& state1, const ParticleEmitterState& state2, float duration) : 
        state1_(state1), state2_(state2), duration_(duration) {
}
    
void TimelineInterpolate::activate() {
    LOG_DEBUG << "timeline interpolate activated, from=" << state1_.name_ << " to=" << state2_.name_ << std::endl;
    timeExpired_ = 0;
}

float TimelineInterpolate::step(float elapsedSeconds) {
    float realStep = (std::min)(duration_ - timeExpired_, elapsedSeconds);
    timeExpired_ += realStep;
    normalizedAge_ = (std::min)(1.0f, timeExpired_ / duration_);
    return elapsedSeconds - realStep;
}

float TimelineInterpolate::numberOfNewParticles(float elapsedSeconds) const {
    float realStep = (std::min)(duration_ - timeExpired_, elapsedSeconds);
    // interpolate emit frequency between the two states
    float freq = (state1_.emitFrequency_ * (1 - normalizedAge_)) + (state2_.emitFrequency_ * normalizedAge_);
    float numNew = freq * realStep;
    return numNew;
}

void TimelineInterpolate::initParticle(Particle& particle, const CL_Vec3f& emitterLocation, float emitterAge) const {
    Particle helper;
    state1_.initParticle(helper, emitterLocation, emitterAge);
    state2_.initParticle(particle, emitterLocation, emitterAge);
    
    particle.startLocation_ = Interpolation::linear(helper.startLocation_, particle.startLocation_, normalizedAge_);
    particle.velocityStart_ = Interpolation::linear(helper.velocityStart_, particle.velocityStart_, normalizedAge_);
    particle.velocityEnd_ = Interpolation::linear(helper.velocityEnd_, particle.velocityEnd_, normalizedAge_);
    particle.lifetimes_[1u] = Interpolation::linear(helper.lifetimes_[1u], particle.lifetimes_[1u], normalizedAge_);
    particle.colorStart_ = Interpolation::linear(helper.colorStart_, particle.colorStart_, normalizedAge_);
    particle.colorEnd_ = Interpolation::linear(helper.colorEnd_, particle.colorEnd_, normalizedAge_);
    particle.sizes_[0u] = Interpolation::linear(helper.sizes_[0u], particle.sizes_[0u], normalizedAge_);
    particle.sizes_[1u] = Interpolation::linear(helper.sizes_[1u], particle.sizes_[1u], normalizedAge_);
}

CL_Vec3f TimelineInterpolate::getEmitterLocationOffset() const {
    // interpolate position between the two states
    return (state1_.emitterLocationOffset_ * (1 - normalizedAge_)) + (state2_.emitterLocationOffset_ * normalizedAge_);
}

float TimelineInterpolate::getMaxParticleLifetime() const {
    return (std::max)(state1_.emittedLifetime_.getMax(), state2_.emittedLifetime_.getMax());
}

}
}
}
