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



#include "serverobject.hpp"

#include <data/manager.hpp>
#include <data/huesloader.hpp>

#include "manager.hpp"
#include "sectormanager.hpp"
#include "sector.hpp"

namespace fluo {
namespace world {


ServerObject::ServerObject(Serial serial, unsigned int objectType) : IngameObject(objectType), serial_(serial), hue_(0) {
    draggable_ = true;
}

Serial ServerObject::getSerial() const {
    return serial_;
}

void ServerObject::setHue(unsigned int hue) {
    if (hue != hue_) {
        hue_ = hue;
        worldRenderData_.hueInfo_[1u] = data::Manager::getHuesLoader()->translateHue(hue_);

        forceRepaint();
    }
}

void ServerObject::onLocationChanged(const CL_Vec3f& oldLocation) {
    Sector* newSector = world::Manager::getSectorManager()->getSectorForCoordinates(getLocX(), getLocY());
    
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

void ServerObject::onDelete() {
    if (sector_) {
        sector_->removeDynamicObject(this);
    }
    sector_ = NULL;
    
    IngameObject::onDelete();
}

}
}
