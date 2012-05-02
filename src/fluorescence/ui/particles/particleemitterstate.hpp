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


#ifndef FLUO_UI_PARTICLES_PARTICLEEMITTERSTATE_HPP
#define FLUO_UI_PARTICLES_PARTICLEEMITTERSTATE_HPP

#include <boost/shared_ptr.hpp>
#include <ClanLib/Display/2D/color.h>
#include <ClanLib/Core/Math/vec2.h>

#include <misc/string.hpp>
#include <misc/random.hpp>

namespace fluo {
namespace ui {
namespace particles {
    
class StartLocationProvider;
class MotionModel;
class Particle;

class ParticleEmitterState {
public:
    UnicodeString name_;
    
    float emitFrequency_;
    
    boost::shared_ptr<StartLocationProvider> emittedStartLocationProvider_;
    boost::shared_ptr<MotionModel> emittedMotionModel_;
    
    RandomizedValue<CL_Colorf> emittedColorStart_;
    RandomizedValue<CL_Colorf> emittedColorEnd_;
    
    RandomizedValue<float> emittedLifetime_;
    
    RandomizedValue<float> emittedSizeStart_;
    RandomizedValue<float> emittedSizeEnd_;
    
    CL_Vec3f emitterLocationOffset_;
    
    float emittedFrameIndexStart_;
    float emittedFrameIndexEnd_;
    
    void initParticle(Particle& particle, const CL_Vec3f& emitterLocation, float emitterAge) const;
};

}
}
}

#endif

