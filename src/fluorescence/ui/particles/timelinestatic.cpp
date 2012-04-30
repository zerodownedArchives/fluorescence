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
 
#include "timelinestatic.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace particles {
    
TimelineStatic::TimelineStatic(const ParticleEmitterState& state, float duration) : state_(state), duration_(duration) {
}
    
void TimelineStatic::activate() {
    LOG_DEBUG << "timeline static activated, state=" << state_.name_ << std::endl;
    timeExpired_ = 0;
}

float TimelineStatic::step(float elapsedSeconds) {
    float realStep = (std::min)(duration_ - timeExpired_, elapsedSeconds);
    timeExpired_ += realStep;
    return elapsedSeconds - realStep;
}

bool TimelineStatic::isExpired() const {
    return timeExpired_ >= duration_;
}

float TimelineStatic::numberOfNewParticles(float elapsedSeconds) const {
    float realStep = (std::min)(duration_ - timeExpired_, elapsedSeconds);
    float numNew = state_.emitFrequency_ * realStep;
    return numNew;
}

void TimelineStatic::initParticle(Particle& particle, const CL_Vec3f& emitterLocation, float emitterAge) const {
    state_.initParticle(particle, emitterLocation, emitterAge);
}

CL_Vec3f TimelineStatic::getEmitterLocationOffset() const {
    return state_.emitterLocationOffset_;
}
    

}
}
}
