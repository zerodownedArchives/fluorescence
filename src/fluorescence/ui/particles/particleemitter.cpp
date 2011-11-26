
#include "particleemitter.hpp"

#include <ClanLib/Display/Render/program_object.h>

namespace fluo {
namespace ui {
namespace particles {

ParticleEmitter::ParticleEmitter(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, unsigned int maxCount,
            float emitPerSec, bool emittedMoveWithEmitter) :
        Emitter(startPos, velStart, velEnd, creationTime, expireTime, maxCount, emitPerSec, emittedMoveWithEmitter),
        particleCount_(0) {

    particles_ = new Particle[maxCount];

    emittedVelocityStartMin_ = CL_Vec3f(50, 0, 0);
    emittedVelocityStartMax_ = CL_Vec3f(79, 20, 0);

    emittedVelocityEndMin_ = emittedVelocityEndMin_ = emittedColorStartMin_;
    emittedVelocityEndMin_.normalize();
    emittedVelocityEndMin_ *= 2;

    emittedVelocityEndMax_ = emittedVelocityEndMax_ = emittedColorStartMax_;
    emittedVelocityEndMax_.normalize();
    emittedVelocityEndMax_ *= 1;

    emittedLifetimeMin_ = 3.0;
    emittedLifetimeMax_ = 5.0;


    emittedColorStartMin_ = CL_Vec4f(0.7, 0.0, 0.0, 0.7);
    emittedColorStartMax_ = CL_Vec4f(1.0, 0.0, 0.0, 1.0);
    emittedColorEndMin_ = CL_Vec4f(0.0, 0.0, 0.7, 0.7);
    emittedColorEndMax_ = CL_Vec4f(0.0, 0.0, 1.0, 1.0);
}

ParticleEmitter::~ParticleEmitter() {
    delete [] particles_;
    particleCount_ = 0;
}

void ParticleEmitter::reset(const CL_Vec3f& startPos, const CL_Vec3f& velStart, const CL_Vec3f& velEnd, float creationTime, float expireTime, unsigned int maxCount,
        float emitPerSec, bool emittedMoveWithEmitter) {

    if (maxCount != emittedMaxCount_) {
        delete [] particles_;
        particles_ = new Particle[maxCount];
        particleCount_ = 0;
    }

    Emitter::reset(startPos, velStart, velEnd, creationTime, expireTime, maxCount, emitPerSec, emittedMoveWithEmitter);
}

unsigned int ParticleEmitter::emittedCount() const {
    return particleCount_;
}

void ParticleEmitter::updateSet(unsigned int newCount, float elapsedSeconds) {
    for (unsigned int i = 0; i < particleCount_; ++i) {
        if (particles_[i].isExpired(age_)) {
            // TODO: if still emitting, reset particle. otherwise remove it
            initParticle(i);
            --newCount;
        }

        // no need to update other particles, that is all done on the gpu
    }

    // create new particles, if necessary
    int remainingNew = (std::min)(newCount, emittedMaxCount_ - emittedCount());
    for (; remainingNew >= 0; --remainingNew) {
        initParticle(particleCount_);
        ++particleCount_;
    }
}

void ParticleEmitter::render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader) {
    // set shader uniform variables
    shader->set_uniform1i("EmittedMoveWithEmitter", emittedMoveWithEmitter_);
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
    particles_[index].reset(
        position_,
        randMinMax(emittedVelocityStartMin_, emittedVelocityStartMax_),
        randMinMax(emittedVelocityEndMin_, emittedVelocityEndMax_),
        age_, // creation time
        age_ + randMinMax(emittedLifetimeMin_, emittedLifetimeMax_),
        randMinMax(emittedColorStartMin_, emittedColorStartMax_),
        randMinMax(emittedColorEndMin_, emittedColorEndMax_)
    );
}

}
}
}
