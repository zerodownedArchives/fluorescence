
#include "particleemitter.hpp"

#include <ClanLib/Display/Render/program_object.h>

#include <misc/random.hpp>
#include <ui/texture.hpp>

#include "startpositionprovider.hpp"
#include "motionmodel.hpp"

namespace fluo {
namespace ui {
namespace particles {

ParticleEmitter::ParticleEmitter(unsigned int maxSize) :
        particleCount_(0) {

    particles_ = new Particle[maxSize];
    startPosition_ = CL_Vec3f(422, 300, 0); // TODO: remove this from here
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
    
    // set shader uniform variables
    shader->set_uniform1i("Texture0", 0);
    CL_Vec3f emitterMovement = emittedMoveWithEmitter_ ? (position_ - startPosition_) : CL_Vec3f(0, 0, 0);
    shader->set_uniform3f("EmitterMovement", emitterMovement);
    shader->set_uniform1f("CurrentTime", age_);
    
    gc.set_texture(0, *(emittedTexture_->getTexture()));

    // collect particle data in primarray
    CL_PrimitivesArray primarray(gc);

    primarray.set_attributes(0, &particles_[0].startPosition_, sizeof(Particle));
    primarray.set_attributes(1, &particles_[0].velocityStart_, sizeof(Particle));
    primarray.set_attributes(2, &particles_[0].velocityEnd_, sizeof(Particle));
    primarray.set_attributes(3, &particles_[0].lifetimes_, sizeof(Particle));
    primarray.set_attributes(4, &particles_[0].colorStart_, sizeof(Particle));
    primarray.set_attributes(5, &particles_[0].colorEnd_, sizeof(Particle));

    // call to draw
    gc.draw_primitives(cl_points, emittedCount(), primarray);
}

void ParticleEmitter::initParticle(unsigned int index) {
    CL_Vec3f particlePosition = emittedStartPositionProvider_->get(position_);

    CL_Vec3f velocity1, velocity2;
    emittedMotionModel_->get(position_, particlePosition, velocity1, velocity2);

    particles_[index].reset(
        particlePosition,
        velocity1,
        velocity2,
        age_, // creation time
        age_ + Random::randomMinMax(emittedLifetimeMin_, emittedLifetimeMax_),
        Random::randomMinMax(emittedColorStartMin_, emittedColorStartMax_),
        Random::randomMinMax(emittedColorEndMin_, emittedColorEndMax_)
    );
}

bool ParticleEmitter::isExpired() const {
    return lifetimes_[0u] + age_ >= lifetimes_[1u] + emittedLifetimeMax_.get();
}

}
}
}
