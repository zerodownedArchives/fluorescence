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
 
#include "timelineimmediate.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace particles {
    
TimelineImmediate::TimelineImmediate(const ParticleEmitterState& state, unsigned int count) : state_(state), count_(count) {
}
    
void TimelineImmediate::activate() {
    LOG_DEBUG << "timeline immediate activated, state=" << state_.name_ << std::endl;
    timeExpired_ = 0;
}

float TimelineImmediate::step(float elapsedSeconds) {
    return elapsedSeconds;
}

bool TimelineImmediate::isExpired() const {
    return true;
}

float TimelineImmediate::numberOfNewParticles(float elapsedSeconds) const {
    return count_;
}

void TimelineImmediate::initParticle(Particle& particle, const CL_Vec3f& emitterLocation, float emitterAge) const {
    state_.initParticle(particle, emitterLocation, emitterAge);
}

CL_Vec3f TimelineImmediate::getEmitterLocationOffset() const {
    return state_.emitterLocationOffset_;
}
    

}
}
}
