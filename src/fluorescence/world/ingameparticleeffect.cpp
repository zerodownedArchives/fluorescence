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



#include "ingameparticleeffect.hpp"

#include <misc/log.hpp>
#include <ui/particles/emitter.hpp>
#include <ui/particles/xmlloader.hpp>

namespace fluo {
namespace world {

IngameParticleEffect::IngameParticleEffect() :
        Effect(IngameObject::TYPE_PARTICLE_EFFECT) {
}

void IngameParticleEffect::update(unsigned int elapsedMillis) {
    Effect::update(elapsedMillis);

    float elapsedSeconds = elapsedMillis / 1000.f;
    CL_Vec3f curLocation = getLocation();

    CL_Vec3f locationPixels(
        (curLocation.x - curLocation.y) * 22 + 22,
        (curLocation.x + curLocation.y) * 22 + 22 - getLocZDraw() * 4,
        0);

    std::list<boost::shared_ptr<ui::particles::Emitter> > expired;
    std::list<boost::shared_ptr<ui::particles::Emitter> >::iterator iter = emitters_.begin();
    std::list<boost::shared_ptr<ui::particles::Emitter> >::iterator end = emitters_.end();

    for (; iter != end; ++iter) {
        (*iter)->setLocation(locationPixels);
        (*iter)->step(elapsedSeconds);

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

bool IngameParticleEffect::shouldExpireTimeout() const {
    if (shouldExplode_) {
        std::list<boost::shared_ptr<ui::particles::Emitter> >::const_iterator iter = emitters_.begin();
        std::list<boost::shared_ptr<ui::particles::Emitter> >::const_iterator end = emitters_.end();

        for (; iter != end; ++iter) {
            if ((*iter)->isEmitting()) {
                return false;
            }
        }
        return true;
    } else {
        return emitters_.empty();
    }
}

void IngameParticleEffect::updateTextureProvider() {
    // do nothing
}

bool IngameParticleEffect::updateAnimation(unsigned int elapsedMillis) {
    // do nothing
    return false;
}

void IngameParticleEffect::updateVertexCoordinates() {
    // do nothing
}

void IngameParticleEffect::updateRenderDepth() {
    // do nothing
}

unsigned int IngameParticleEffect::startExplosion() {
    event("targetreached");
    return 0;
}

boost::shared_ptr<ui::Texture> IngameParticleEffect::getIngameTexture() const {
    boost::shared_ptr<ui::Texture> ret;
    return ret;
}

}
}

