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


#ifndef FLUO_UI_PARTICLES_TIMELINE_HPP
#define FLUO_UI_PARTICLES_TIMELINE_HPP

#include <vector>
#include <boost/shared_ptr.hpp>
#include <ClanLib/Core/Math/vec3.h>

namespace fluo {
namespace ui {
namespace particles {
    
class ParticleEmitter;
class TimelineElement;
class Particle;

class Timeline {
public:
    Timeline(ParticleEmitter* parent);
    
    void step(float elapsedSeconds);
    
    bool isExpired() const;
    bool isEmitting() const;
    
    void initParticle(Particle& particle, const CL_Vec3f& emitterLocation, float emitterAge) const;
    CL_Vec2f getEmitterLocationOffset() const;
    
    void addElement(boost::shared_ptr<TimelineElement> elem);

private:
    ParticleEmitter* emitter_;
    
    std::vector<boost::shared_ptr<TimelineElement> > elements_;
    unsigned int currentIndex_;
    TimelineElement* currentElement_;
    
    float maxParticleLifetime_;
    float waitAfterLastEmit_;
};

}
}
}

#endif

