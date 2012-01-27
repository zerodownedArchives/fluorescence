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

