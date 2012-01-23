
#include "particleeffect.hpp"

#include <misc/log.hpp>
#include <ui/particles/emitter.hpp>

namespace fluo {
namespace world {

ParticleEffect::ParticleEffect() : IngameObject(IngameObject::TYPE_PARTICLE_EFFECT) {
}

bool ParticleEffect::hasPixel(int pixelX, int pixelY) const {
    return false;
}

bool ParticleEffect::isInDrawArea(int leftPixelCoord, int rightPixelCoord, int topPixelCoord, int bottomPixelCoord) const {
    return true;
}

boost::shared_ptr<ui::Texture> ParticleEffect::getIngameTexture() const {
    static boost::shared_ptr<ui::Texture> ret;
    return ret;
}

void ParticleEffect::update(unsigned int elapsedMillis) {
    float elapsedSeconds = elapsedMillis / 1000.f;
    
    std::list<boost::shared_ptr<ui::particles::Emitter> > expired;
    
    for (boost::shared_ptr<ui::particles::Emitter>& em : emitters_) {
        em->update(elapsedSeconds);
        
        if (em->isExpired()) {
            expired.push_back(em);
        }
    }
    
    if (!expired.empty()) {
        for (boost::shared_ptr<ui::particles::Emitter>& ex : expired) {
            emitters_.remove(ex);
        }
    }
}

void ParticleEffect::renderAll(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader) {
    for (boost::shared_ptr<ui::particles::Emitter>& em : emitters_) {
        em->render(gc, shader);
    }
}

void ParticleEffect::addEmitter(boost::shared_ptr<ui::particles::Emitter> emitter) {
    emitters_.push_back(emitter);
}

void ParticleEffect::updateTextureProvider() {
}

bool ParticleEffect::updateAnimation(unsigned int elapsedMillis) {
    return true;
}

void ParticleEffect::updateVertexCoordinates() {
}

void ParticleEffect::updateRenderDepth() {
}


}
}

