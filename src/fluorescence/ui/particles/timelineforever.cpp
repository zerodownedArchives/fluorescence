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
 
#include "timelineforever.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace particles {
    
TimelineForever::TimelineForever(const ParticleEmitterState& state) : state_(state) {
}
    
void TimelineForever::activate() {
    LOG_DEBUG << "timeline forever activated, state=" << state_.name_ << std::endl;
}

float TimelineForever::step(float elapsedSeconds) {
    return 0.0;
}

float TimelineForever::numberOfNewParticles(float elapsedSeconds) const {
    float numNew = state_.emitFrequency_ * elapsedSeconds;
    return numNew;
}

void TimelineForever::initParticle(Particle& particle, const CL_Vec3f& emitterLocation, float emitterAge) const {
    state_.initParticle(particle, emitterLocation, emitterAge);
}

CL_Vec3f TimelineForever::getEmitterLocationOffset() const {
    return state_.emitterLocationOffset_;
}
 
float TimelineForever::getMaxParticleLifetime() const {
    return state_.emittedLifetime_.getMax();
}

}
}
}
