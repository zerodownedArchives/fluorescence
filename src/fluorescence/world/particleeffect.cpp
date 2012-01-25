
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

    std::list<boost::shared_ptr<ui::particles::Emitter> >::iterator iter = emitters_.begin();
    std::list<boost::shared_ptr<ui::particles::Emitter> >::iterator end = emitters_.end();
    
    for (; iter != end; ++iter) {
        (*iter)->update(elapsedSeconds);
        
        if ((*iter)->isExpired()) {
            expired.push_back(*iter);
        }
    }

    if (!expired.empty()) {
        iter = expired.begin();
        end = expired.end();

        for (; iter != end; ++iter) {
            emitters_.remove(*iter);
        }
    }
}

void ParticleEffect::renderAll(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader) {
    std::list<boost::shared_ptr<ui::particles::Emitter> >::iterator iter = emitters_.begin();
    std::list<boost::shared_ptr<ui::particles::Emitter> >::iterator end = emitters_.end();

    for (; iter != end; ++iter) {
        (*iter)->render(gc, shader);
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

bool ParticleEffect::isExpired() const {
    return emitters_.empty();
}


}
}

