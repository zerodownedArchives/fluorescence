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


#ifndef FLUO_UI_PARTICLES_PARTICLEEMITTER_HPP
#define FLUO_UI_PARTICLES_PARTICLEEMITTER_HPP

#include <ClanLib/Display/Render/texture.h>

#include "emitter.hpp"
#include "particle.hpp"
#include "timeline.hpp"

namespace fluo {
namespace ui {

class Texture;
    
namespace particles {
    
class XmlLoader;

class ParticleEmitter : public Emitter {

friend class XmlLoader;

public:
    ParticleEmitter(unsigned int capacity);
    ~ParticleEmitter();

    virtual void step(float elapsedSeconds);
    virtual void render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader);
    
    virtual bool isExpired() const;
    virtual bool isEmitting() const;
    
    void emitParticles(float count);
    
    virtual void onEvent(const UnicodeString& event);

private:
    unsigned int capacity_;
    unsigned int particleCount_;
    Particle* particles_;
    unsigned int newEmitIndex_;

    boost::shared_ptr<ui::Texture> emittedTexture_;
    
    Timeline timeline_;
    
    void updateRemainingSet();
    void removeParticle(unsigned int index);
};

}
}
}

#endif
