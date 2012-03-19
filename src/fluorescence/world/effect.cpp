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

#include "effect.hpp"

#include <iomanip>

#include "sector.hpp"
#include "sectormanager.hpp"
#include "manager.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace world {
    
Effect::Effect(unsigned int ingameObjectType) :
        IngameObject(ingameObjectType), expired_(false), lifetimeMillisLeft_(0), shouldExplode_(false), exploding_(false), fixedAngle_(true), currentAngle_(0) {
}

void Effect::setMoving(boost::shared_ptr<IngameObject> source, boost::shared_ptr<IngameObject> target, unsigned int speed) {
    sourceObject_ = source;
    targetObject_ = target;
    speed_ = speed;
    float dist = (target->getLocation() - source->getLocation()).length();
    speedPerSecond_ = dist * (speed_ / 2.f);
    if (speedPerSecond_ < dist) {
        speedPerSecond_ = dist;
    }
    setLocation(source->getLocation());
    effectType_ = TYPE_SOURCE_TO_TARGET;
}

void Effect::setLightning(boost::shared_ptr<IngameObject> source, boost::shared_ptr<IngameObject> target) {
    sourceObject_ = source;
    targetObject_ = target;
    effectType_ = TYPE_LIGHTNING;
    
    // ligthing effect is composed of 10 gump graphics starting from 0x4e20 (20000)
}

void Effect::setAtPosition(unsigned int x, unsigned int y, int z) {
    setLocation(CL_Vec3f(x, y, z));
    effectType_ = TYPE_AT_POSITION;
}

void Effect::setAtSource(boost::shared_ptr<IngameObject> source) {
    sourceObject_ = source;
    effectType_ = TYPE_AT_SOURCE;
}

void Effect::setLifetimeMillis(unsigned int millis) {
    lifetimeMillisLeft_ = millis;
}

void Effect::update(unsigned int elapsedMillis) {
    switch (effectType_) {
        case TYPE_SOURCE_TO_TARGET: {
            boost::shared_ptr<IngameObject> targetObj = targetObject_.lock();
            boost::shared_ptr<IngameObject> sourceObj = sourceObject_.lock();
            if (!targetObj || !sourceObj) {
                expired_ = true;
                break;
            }
            
            if (exploding_) {
                if (getLocation() != targetObj->getLocation()) {
                    setLocation(targetObj->getLocation());
                }
            } else {
                // interpolate
                // movement is not exactly like osi, but close enough
                float timeFactor = elapsedMillis / 1000.f * speedPerSecond_;
                
                CL_Vec3f targetLoc = targetObj->getLocation();
                CL_Vec3f selfLoc = getLocation();
                CL_Vec3f totalDistance = targetLoc - selfLoc;
                
                if (totalDistance.length() <= timeFactor) {
                    // reached target
                    setLocation(targetLoc);
                    lifetimeMillisLeft_ = -1;
                    
                    fixedAngle_ = true;
                    currentAngle_ = 0;
                } else {
                    CL_Vec3f step = totalDistance.normalize() * timeFactor;
                    selfLoc += step;
                    setLocation(selfLoc);
                    
                    // does not expire while moving
                    lifetimeMillisLeft_ = elapsedMillis;
                    
                    if (!fixedAngle_) {
                        // this has to be in screen coordinates
                        totalDistance -= step;
                        float screenDiffX = (totalDistance.x - totalDistance.y) * 44;
                        float screenDiffY = (totalDistance.x + totalDistance.y) * 44 + totalDistance.z * 4;
                        
                        currentAngle_ = atan2(screenDiffY, screenDiffX) + 3.14159265;;
                    }
                }
            }
            break;
        }
            
        case TYPE_LIGHTNING:
            break;
            
        case TYPE_AT_POSITION:
            break;
            
        case TYPE_AT_SOURCE: {
            boost::shared_ptr<IngameObject> obj = sourceObject_.lock();
            if (!obj) {
                expired_ = true;
            } else {
                if (getLocation() != obj->getLocation()) {
                    setLocation(obj->getLocation());
                }
            }
            break;
        }
    }
    
    // check expire time
    lifetimeMillisLeft_ -= elapsedMillis;
    if (lifetimeMillisLeft_ < 0) {
        if (shouldExplode_) {
            shouldExplode_ = false;
            exploding_ = true;
            lifetimeMillisLeft_ = startExplosion();
        } else {
            expired_ = true;
        }
    }
}

bool Effect::isExpired() const {
    return expired_;
}

void Effect::onDelete() {
    if (sector_) {
        sector_->removeDynamicObject(this);
    }
    
    sector_.reset();
    
    IngameObject::onDelete();
}

void Effect::onLocationChanged(const CL_Vec3f& oldLocation) {
    boost::shared_ptr<Sector> newSector = world::Manager::getSectorManager()->getSectorForCoordinates(getLocXGame(), getLocYGame());
    
    if (sector_ != newSector) {
        if (sector_) {
            sector_->removeDynamicObject(this);
        }
        
        if (newSector) {
            newSector->addDynamicObject(this);
        }
    }
    
    sector_ = newSector;
}

void Effect::setShouldExplode(bool value) {
    shouldExplode_ = value;
}

void Effect::setFixedAngle(bool value) {
    fixedAngle_ = value;
}

}
}
