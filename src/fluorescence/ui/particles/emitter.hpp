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


#ifndef FLUO_UI_PARTICLES_EMITTER_HPP
#define FLUO_UI_PARTICLES_EMITTER_HPP

#include "emittable.hpp"

#include <boost/shared_ptr.hpp>
#include <ClanLib/Display/Render/graphic_context.h>
class CL_ProgramObject;

#include <misc/interpolation.hpp>

namespace fluo {
namespace ui {
namespace particles {

class StartPositionProvider;
class MotionModel;

class Emitter : public Emittable {
public:
    Emitter();

    // updateSet is used to propagate the change internally (e.g. to children)
    virtual void update(float elapsedSeconds);

    virtual unsigned int emittedCount() const = 0;

    virtual void render(CL_GraphicContext& gc, boost::shared_ptr<CL_ProgramObject>& shader) = 0;
    
    virtual bool isExpired() const = 0;
    bool isEmitting() const;


protected:
    virtual void updateSet(unsigned int newCount, float elapsedSeconds) = 0;

    unsigned int emittedStartCount_;
    unsigned int emittedMaxCount_;
    InterpolatedValue<float> emitPerSecond_;

    CL_Vec3f position_;

    float age_;
    float normalizedAge_;
    float emittedFractionStore_;


    // parameters for the emitted objects
    InterpolatedValue<float> emittedLifetimeMin_;
    InterpolatedValue<float> emittedLifetimeMax_;

    bool emittedMoveWithEmitter_;

    boost::shared_ptr<StartPositionProvider> emittedStartPositionProvider_;
    boost::shared_ptr<MotionModel> emittedMotionModel_;
};

}
}
}

#endif
