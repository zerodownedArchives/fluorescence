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

#include "sector.hpp"
#include "sectormanager.hpp"
#include "manager.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace world {
    
Effect::Effect(unsigned int ingameObjectType) :
        IngameObject(ingameObjectType), expired_(false), lifetimeMillisLeft_(0) {
}

void Effect::setMoving(boost::shared_ptr<IngameObject> source, boost::shared_ptr<IngameObject> target) {
    sourceObject_ = source;
    targetObject_ = target;
    effectType_ = TYPE_SOURCE_TO_TARGET;
}

void Effect::setLightning(boost::shared_ptr<IngameObject> source, boost::shared_ptr<IngameObject> target) {
    sourceObject_ = source;
    targetObject_ = target;
    effectType_ = TYPE_LIGHTNING;
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
        case TYPE_SOURCE_TO_TARGET:
            // interpolate
            break;
            
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
        expired_ = true;
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

}
}
