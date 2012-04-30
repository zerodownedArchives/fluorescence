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

ParticleEmitter::ParticleEmitter(unsigned int capacity) :
        capacity_(capacity), particleCount_(0), timeline_(this) {

    particles_ = new Particle[capacity_];
}

ParticleEmitter::~ParticleEmitter() {
    delete [] particles_;
    particleCount_ = 0;
}

void ParticleEmitter::step(float elapsedSeconds) {
    age_ += elapsedSeconds;
    newEmitIndex_ = 0;
    timeline_.step(elapsedSeconds);
    updateRemainingSet();
}

void ParticleEmitter::emitParticles(float count) {
    float newCountF = count + emittedFractionStore_;
    unsigned int newCount = newCountF;
    emittedFractionStore_ = newCountF - newCount;
    
    if (newCount == 0) {
        return;
    }
    
    CL_Vec3f paramLocation = emittedMoveWithEmitter_ ? getStartLocation() : getLocation();
    
    // reuse already existing particles
    for(; newEmitIndex_ < particleCount_ && newCount > 0; ++newEmitIndex_) {
        if (particles_[newEmitIndex_].isExpired(age_)) {
            timeline_.initParticle(particles_[newEmitIndex_], paramLocation, age_);
            --newCount;
        }
    }
    
    // increase particle count if necessary
    int remainingNew = (std::min)(newCount, capacity_ - particleCount_);
    particleCount_ += remainingNew;
    for (; remainingNew > 0; --remainingNew) {
        timeline_.initParticle(particles_[newEmitIndex_], paramLocation, age_);
        ++newEmitIndex_;
    }
}

void ParticleEmitter::updateRemainingSet() {
    for (; newEmitIndex_ < particleCount_; ++newEmitIndex_) {
        if (particles_[newEmitIndex_].isExpired(age_)) {
            removeParticle(newEmitIndex_);
        }
    }
    
    // to increase performance all unused particles at the end of the
    // set could be cut off
}

void ParticleEmitter::removeParticle(unsigned int index) {
    particles_[index].startLocation_.z = 10.0; // particle does not pass depth test then
}

void ParticleEmitter::render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader) {
    if (!emittedTexture_ || !emittedTexture_->isReadComplete()) {
        return;
    }
    
    LOG_DEBUG << "render, particlecount=" << particleCount_ << std::endl;
    
    if (extractedTexture_.is_null()) {
        // create CL_Texture from ui::Texture. can be very expensive, if the CL_Subtexture was already initialized.
        extractedTexture_ = emittedTexture_->extractSingleTexture();
    }
    
    // set shader uniform variables
    shader->set_uniform1i("Texture0", 0);
    CL_Vec3f emitterMovement = emittedMoveWithEmitter_ ? (getLocation() - getStartLocation()) : CL_Vec3f(0, 0, 0);
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
    gc.draw_primitives(cl_points, particleCount_, primarray);
}

bool ParticleEmitter::isExpired() const {
    return timeline_.isExpired();
}

bool ParticleEmitter::isEmitting() const {
    return timeline_.isEmitting();
}

}
}
}
