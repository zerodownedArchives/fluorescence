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


#ifndef FLUO_WORLD_EFFECT_HPP
#define FLUO_WORLD_EFFECT_HPP

#include <boost/weak_ptr.hpp>

#include "ingameobject.hpp"

namespace fluo {
namespace world {

class Effect : public IngameObject {
public:
    enum {
        TYPE_SOURCE_TO_TARGET = 0,
        TYPE_LIGHTNING = 1,
        TYPE_AT_POSITION = 2,
        TYPE_AT_SOURCE = 3,
    };

    Effect(unsigned int ingameObjectType);
    
    void setMoving(boost::shared_ptr<IngameObject> source, boost::shared_ptr<IngameObject> target, unsigned int speed);
    void setLightning(boost::shared_ptr<IngameObject> source, boost::shared_ptr<IngameObject> target);
    void setAtPosition(unsigned int x, unsigned int y, int z);
    void setAtSource(boost::shared_ptr<IngameObject> source);
    
    void setLifetimeMillis(unsigned int millis);
    
    virtual void onLocationChanged(const CL_Vec3f& oldLocation);
    
    void update(unsigned int elapsedMillis);
    
    bool isExpired() const;
    
    void onDelete();

    void setShouldExplode(bool value);
    void setFixedAngle(bool value);
    
    // returns the explosion duration in milliseconds
    virtual unsigned int startExplosion() = 0;

protected:
    unsigned int effectType_;
    
    boost::weak_ptr<IngameObject> sourceObject_;
    boost::weak_ptr<IngameObject> targetObject_;
    
    bool expired_;
    int lifetimeMillisLeft_;
    
    bool shouldExplode_;
    bool exploding_;

    unsigned int speed_;
    float speedPerSecond_;
    
    bool fixedAngle_;
    float currentAngle_;
};
    
}
}

#endif

