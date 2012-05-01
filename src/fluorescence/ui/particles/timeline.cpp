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

#include "timeline.hpp"

#include <misc/log.hpp>

#include "timelineelement.hpp"
#include "particleemitter.hpp"
#include "timelineevent.hpp"

namespace fluo {
namespace ui {
namespace particles {

Timeline::Timeline(ParticleEmitter* emitter) : 
        emitter_(emitter), currentIndex_(0xFFFFFFFFu), currentElement_(nullptr), maxParticleLifetime_(0), repeatCount_(0) {
}

void Timeline::step(float elapsedSeconds) {
    if (isExpired()) {
        return;
    } else if (!isEmitting()) {
        waitAfterLastEmit_ -= elapsedSeconds;
        return;
    }
    
    float secondsLeft = elapsedSeconds;
    
    while (secondsLeft > 0) {
        float numNewParticles = currentElement_->numberOfNewParticles(elapsedSeconds);
        secondsLeft = currentElement_->step(elapsedSeconds);
        
        emitter_->setLocationOffset(currentElement_->getEmitterLocationOffset());
        if (numNewParticles > 0) {
            emitter_->emitParticles(numNewParticles);
        }

        if (secondsLeft > 0) {
            ++currentIndex_;
            if (!isEmitting()) {
                // timeline end reached, check if the whole timeline should be repeated
                if (repeatCount_ != 0) {
                    currentIndex_ = 0;
                    if (repeatCount_ > 0) { // can also be -1 for endless repeat
                        --repeatCount_;
                    }
                } else {
                    // set timer to wait for all particles to disperse
                    waitAfterLastEmit_ = maxParticleLifetime_;
                    break;
                }
            }
            currentElement_ = elements_[currentIndex_].get();
            currentElement_->activate();
        }
    }
}

bool Timeline::isExpired() const {
    return !isEmitting() && waitAfterLastEmit_ <= 0;
}

bool Timeline::isEmitting() const {
    return currentIndex_ < elements_.size();
}

void Timeline::initParticle(Particle& particle, const CL_Vec3f& emitterLocation, float emitterAge) const {
    currentElement_->initParticle(particle, emitterLocation, emitterAge);
}

void Timeline::addElement(boost::shared_ptr<TimelineElement> elem) {
    elements_.push_back(elem);
    
    // first element
    if (currentIndex_ == 0xFFFFFFFFu) {
        currentIndex_ = 0;
        currentElement_ = elements_[currentIndex_].get();
        currentElement_->activate();
    }
    
    maxParticleLifetime_ = (std::max)(maxParticleLifetime_, elem->getMaxParticleLifetime());
}

CL_Vec2f Timeline::getEmitterLocationOffset() const {
    return currentElement_->getEmitterLocationOffset();
}

void Timeline::onEvent(const UnicodeString& event) {
    unsigned int eventElemIndex = currentIndex_ + 1;
    
    LOG_DEBUG << "Timeline::onEvent: " << event << std::endl;
    
    for (; eventElemIndex < elements_.size(); ++eventElemIndex) {
        boost::shared_ptr<TimelineEvent> eventElem = boost::dynamic_pointer_cast<TimelineEvent>(elements_[eventElemIndex]);
        LOG_DEBUG << "index: " << eventElemIndex << " isEvent: " << (bool)eventElem << std::endl;
        if (eventElem && eventElem->consumesEvent(event)) {
            currentIndex_ = eventElemIndex;
            currentElement_ = eventElem.get();
            currentElement_->activate();
            break;
        }
    }
}

void Timeline::setRepeat(int repeat) {
    if (repeat > 0) {
        --repeat;
    }
    repeatCount_ = repeat;
}

}
}
}
