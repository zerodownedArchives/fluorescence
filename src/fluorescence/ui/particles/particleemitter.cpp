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



#include "particleemitter.hpp"

#include <ClanLib/Display/Render/texture.h>
#include <ClanLib/Display/Render/program_object.h>
#include <ClanLib/Display/2D/draw.h>

#include <misc/random.hpp>
#include <misc/log.hpp>
#include <ui/texture.hpp>

#include "startlocationprovider.hpp"
#include "motionmodel.hpp"

namespace fluo {
namespace ui {
namespace particles {

ParticleEmitter::ParticleEmitter(unsigned int maxSize) :
        particleCount_(0) {

    particles_ = new Particle[maxSize];
}

ParticleEmitter::~ParticleEmitter() {
    delete [] particles_;
    particleCount_ = 0;
}

unsigned int ParticleEmitter::emittedCount() const {
    return particleCount_;
}

void ParticleEmitter::updateSet(unsigned int newCount, float elapsedSeconds) {
    emittedColorStartMin_.setNormalizedAge(normalizedAge_);
    emittedColorStartMax_.setNormalizedAge(normalizedAge_);
    emittedColorEndMin_.setNormalizedAge(normalizedAge_);
    emittedColorEndMax_.setNormalizedAge(normalizedAge_);

    if (emittedStartCount_ > 0) {
        for (unsigned int i = 0; i < emittedStartCount_; ++i) {
            initParticle(i);
            ++particleCount_;
        }
        emittedStartCount_ = 0;
    }

    bool shouldEmitNew = isEmitting();

    for (unsigned int i = 0; i < particleCount_; ++i) {
        if (particles_[i].isExpired(age_)) {
            if (shouldEmitNew) {
                // if still emitting, reset particle
                initParticle(i);

                if (newCount > 0) {
                    --newCount;
                }
            } else if (!particles_[i].isRemoved()) {
                // remove particle
                particles_[i].remove();
            }
        }

        // no need to update particle attributes, that is all done on the gpu
    }

    // create new particles, if necessary
    if (shouldEmitNew) {
        int remainingNew = (std::min)(newCount, emittedMaxCount_ - emittedCount());
        for (; remainingNew > 0; --remainingNew) {
            initParticle(particleCount_);
            ++particleCount_;
        }
    }
}

void ParticleEmitter::render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader) {
    if (!emittedTexture_ || !emittedTexture_->isReadComplete()) {
        return;
    }
    
    if (extractedTexture_.is_null()) {
        // create CL_Texture from ui::Texture. can be very expensive, if the CL_Subtexture was already initialized.
        extractedTexture_ = emittedTexture_->extractSingleTexture();
    }
    
    // set shader uniform variables
    shader->set_uniform1i("Texture0", 0);
    CL_Vec3f emitterMovement = emittedMoveWithEmitter_ ? (location_ - startLocation_) : CL_Vec3f(0, 0, 0);
    shader->set_uniform3f("EmitterMovement", emitterMovement);
    shader->set_uniform1f("CurrentTime", age_);
    
    gc.set_texture(0, extractedTexture_);

    // collect particle data in primarray
    CL_PrimitivesArray primarray(gc);

    primarray.set_attributes(0, &particles_[0].startLocation_, sizeof(Particle));
    primarray.set_attributes(1, &particles_[0].velocityStart_, sizeof(Particle));
    primarray.set_attributes(2, &particles_[0].velocityEnd_, sizeof(Particle));
    primarray.set_attributes(3, &particles_[0].lifetimes_, sizeof(Particle));
    primarray.set_attributes(4, &particles_[0].colorStart_, sizeof(Particle));
    primarray.set_attributes(5, &particles_[0].colorEnd_, sizeof(Particle));

    // call to draw
    gc.draw_primitives(cl_points, emittedCount(), primarray);
}

void ParticleEmitter::initParticle(unsigned int index) {
    CL_Vec3f particleLocation = emittedStartLocationProvider_->get(location_);

    CL_Vec3f velocity1, velocity2;
    emittedMotionModel_->get(location_, particleLocation, velocity1, velocity2);

    particles_[index].reset(
        particleLocation,
        velocity1,
        velocity2,
        age_, // creation time
        age_ + Random::randomMinMax(emittedLifetimeMin_, emittedLifetimeMax_),
        Random::randomMinMax(emittedColorStartMin_, emittedColorStartMax_),
        Random::randomMinMax(emittedColorEndMin_, emittedColorEndMax_)
    );
}

bool ParticleEmitter::isExpired() const {
    //LOG_DEBUG << "ParticleEmitter::isExpired: " << lifetimes_[0u] << " + " << age_ << " >= " << lifetimes_[1u] << " + " << emittedLifetimeMax_.get() << std::endl;
    return lifetimes_[0u] + age_ >= lifetimes_[1u] + emittedLifetimeMax_.get();
}

}
}
}
