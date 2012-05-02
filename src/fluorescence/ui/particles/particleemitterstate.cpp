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


#include "particleemitterstate.hpp"

#include "particle.hpp"
#include "startlocationprovider.hpp"
#include "motionmodel.hpp"

namespace fluo {
namespace ui {
namespace particles {
    
void ParticleEmitterState::initParticle(Particle& particle, const CL_Vec3f& emitterLocation, float emitterAge) const {
    CL_Vec3f particleLocation = emittedStartLocationProvider_->get(emitterLocation);

    CL_Vec3f velocity1, velocity2;
    emittedMotionModel_->get(emitterLocation, particleLocation, velocity1, velocity2);

    particle.startLocation_ = particleLocation;
    particle.velocityStart_ = velocity1;
    particle.velocityEnd_ = velocity2;
    particle.lifetimes_ = emitterAge; // creation time
    particle.lifetimes_[1u] = particle.lifetimes_[0u] + emittedLifetime_.get(); // expire time
    CL_Colorf col = emittedColorStart_.get();
    particle.colorStart_ = CL_Vec4f(col.r, col.g, col.b, col.a);
    col = emittedColorEnd_.get();
    particle.colorEnd_ = CL_Vec4f(col.r, col.g, col.b, col.a);
    particle.sizes_[0u] = emittedSizeStart_.get();
    particle.sizes_[1u] = emittedSizeEnd_.get();
    particle.frameIndices_[0u] = emittedFrameIndexStart_;
    particle.frameIndices_[1u] = emittedFrameIndexEnd_;
}

}
}
}
