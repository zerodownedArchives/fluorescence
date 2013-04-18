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

#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>

#include <data/manager.hpp>
#include <data/clilocloader.hpp>
#include <data/huesloader.hpp>

#include <net/manager.hpp>
#include <net/packets/d6_objectpropertyrequest.hpp>

#include <ui/manager.hpp>
#include <ui/python/scriptloader.hpp>

#include "manager.hpp"
#include "sectormanager.hpp"
#include "sector.hpp"

namespace fluo {
namespace world {


ServerObject::ServerObject(Serial serial, unsigned int objectType) : IngameObject(objectType),
        serial_(serial), hue_(0), clilocPropertiesHash_(0) {
    draggable_ = true;
    requestClilocProperties();
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

void ServerObject::onDelete() {
    if (sector_) {
        sector_->removeDynamicObject(this);
    }

    sector_.reset();

    IngameObject::onDelete();
}

void ServerObject::clearClilocProperties() {
    clilocProperties_.clear();
}

void ServerObject::addClilocProperty(unsigned int cliloc, const UnicodeString& args) {
    clilocProperties_.push_back(data::Manager::getClilocLoader()->get(cliloc, args));
}

void ServerObject::setClilocPropertiesHash(uint32_t hash) {
    clilocPropertiesHash_ = hash;
}

bool ServerObject::checkClilocPropertiesHash(uint32_t hash) {
    return clilocPropertiesHash_ != hash;
}

void ServerObject::requestClilocProperties() {
    clearClilocProperties();
    net::packets::ObjectPropertyRequest req(serial_);
    net::Manager::getSingleton()->send(req);
}

void ServerObject::openPropertyListGump(const CL_Point& mousePos) {
    if (clilocProperties_.empty()) {
        return;
    }

    try {
        boost::python::dict args;
        args["x"] = mousePos.x + 1;
        args["y"] = mousePos.y + 1;

        boost::python::list lines;
        std::list<UnicodeString>::iterator iter = clilocProperties_.begin();
        std::list<UnicodeString>::iterator end = clilocProperties_.end();
        for (; iter != end; ++iter) {
            lines.append(*iter);
        }
        args["lines"] = lines;

        ui::Manager::getSingleton()->openPythonGump("objectpropertylist", args);
    } catch (const boost::python::error_already_set& err) {
        ui::Manager::getSingleton()->getPythonLoader()->logError();
        throw;
    }
}

}
}
