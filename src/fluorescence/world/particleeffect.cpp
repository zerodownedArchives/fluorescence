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

ParticleEffect::ParticleEffect() :
        Effect(IngameObject::TYPE_PARTICLE_EFFECT) {
}

void ParticleEffect::update(unsigned int elapsedMillis) {
    Effect::update(elapsedMillis);
    
    float elapsedSeconds = elapsedMillis / 1000.f;
    //CL_Vec3f curLocation = getLocation();
    
    CL_Vec3f locationPixels(
        300, //(curLocation.x - curLocation.y) * 22 + 22,
        300, //(curLocation.x + curLocation.y) * 22 + 22,
        0);
    
    std::list<boost::shared_ptr<ui::particles::Emitter> > expired;
    std::list<boost::shared_ptr<ui::particles::Emitter> >::iterator iter = emitters_.begin();
    std::list<boost::shared_ptr<ui::particles::Emitter> >::iterator end = emitters_.end();
    
    for (; iter != end; ++iter) {
        (*iter)->setLocation(locationPixels);
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
    LOG_DEBUG << "renderAll particle effect " << getLocation() << std::endl;
    std::list<boost::shared_ptr<ui::particles::Emitter> >::iterator iter = emitters_.begin();
    std::list<boost::shared_ptr<ui::particles::Emitter> >::iterator end = emitters_.end();

    for (; iter != end; ++iter) {
        (*iter)->render(gc, shader);
    }
}

void ParticleEffect::addEmitter(boost::shared_ptr<ui::particles::Emitter> emitter) {
    emitters_.push_back(emitter);
}

bool ParticleEffect::isExpired() const {
    return emitters_.empty();
}

void ParticleEffect::updateTextureProvider() {
    // do nothing
}

bool ParticleEffect::updateAnimation(unsigned int elapsedMillis) {
    // do nothing
    return false;
}

void ParticleEffect::updateVertexCoordinates() {
    // do nothing
}

void ParticleEffect::updateRenderDepth() {
    // do nothing
}

unsigned int ParticleEffect::startExplosion() {
    // TODO
    return 0;
}

boost::shared_ptr<ui::Texture> ParticleEffect::getIngameTexture() const {
    boost::shared_ptr<ui::Texture> ret;
    return ret;
}

}
}

