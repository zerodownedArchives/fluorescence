
#include "particleemitter.hpp"

#include <ClanLib/Display/Render/program_object.h>

#include <misc/random.hpp>

#include "startpositionprovider.hpp"
#include "motionmodel.hpp"

namespace fluo {
namespace ui {
namespace particles {

ParticleEmitter::ParticleEmitter(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd,
            float creationTime, float expireTime,
            unsigned int startCount, unsigned int maxCount, float emitPerSec,
            bool emittedMoveWithEmitter,
            const boost::shared_ptr<StartPositionProvider>& emittedStartPosProvider,
            const boost::shared_ptr<MotionModel>& emittedMotionModel) :
        Emitter(startPos, velStart, velEnd, creationTime, expireTime, startCount, maxCount, emitPerSec, emittedMoveWithEmitter, emittedStartPosProvider, emittedMotionModel),
        particleCount_(0) {

    particles_ = new Particle[maxCount];

    emittedLifetimeMin_ = 1.0;
    emittedLifetimeMax_ = 2.0;

    emittedColorStartMin_ = CL_Vec4f(0.9, 0.65, 0.0, 0.4);
    emittedColorStartMax_ = CL_Vec4f(1.0, 0.85, 0.0, 0.6);
    emittedColorEndMin_ = CL_Vec4f(0.8, 0.0, 0.0, 0.0);
    emittedColorEndMax_ = CL_Vec4f(1.0, 0.1, 0.0, 0.2);
}

ParticleEmitter::~ParticleEmitter() {
    delete [] particles_;
    particleCount_ = 0;
}

void ParticleEmitter::reset(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd,
            float creationTime, float expireTime,
            unsigned int startCount, unsigned int maxCount, float emitPerSec,
            bool emittedMoveWithEmitter,
            const boost::shared_ptr<StartPositionProvider>& emittedStartPosProvider,
            const boost::shared_ptr<MotionModel>& emittedMotionModel) {

    if (maxCount != emittedMaxCount_) {
        delete [] particles_;
        particles_ = new Particle[maxCount];
        particleCount_ = 0;
    }

    Emitter::reset(startPos, velStart, velEnd, creationTime, expireTime, startCount, maxCount, emitPerSec, emittedMoveWithEmitter, emittedStartPosProvider, emittedMotionModel);
}

unsigned int ParticleEmitter::emittedCount() const {
    return particleCount_;
}

void ParticleEmitter::updateSet(unsigned int newCount, float elapsedSeconds) {
    if (emittedStartCount_ > 0) {
        for (unsigned int i = 0; i < emittedStartCount_; ++i) {
            initParticle(i);
            ++particleCount_;
        }
        emittedStartCount_ = 0;
    }

    for (unsigned int i = 0; i < particleCount_; ++i) {
        if (particles_[i].isExpired(age_)) {
            if (!isExpired(age_)) {
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
    if (!isExpired(age_)) {
        int remainingNew = (std::min)(newCount, emittedMaxCount_ - emittedCount());
        for (; remainingNew > 0; --remainingNew) {
            initParticle(particleCount_);
            ++particleCount_;
        }
    }
}

void ParticleEmitter::render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader) {
    // set shader uniform variables
    shader->set_uniform1i("Texture0", 0);
    CL_Vec3f emitterMovement = emittedMoveWithEmitter_ ? (position_ - startPosition_) : CL_Vec3f(0, 0, 0);
    shader->set_uniform3f("EmitterMovement", emitterMovement);
    shader->set_uniform1f("CurrentTime", age_);

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

}
}
}
