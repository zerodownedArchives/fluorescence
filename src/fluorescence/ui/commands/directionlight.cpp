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

#include "directionlight.hpp"

#include <typedefs.hpp>
#include <world/manager.hpp>
#include <world/lightmanager.hpp>

namespace fluo {
namespace ui {
namespace commands {
    
void DirectionLight::execute(const UnicodeString& args) {
    boost::shared_ptr<world::LightManager> lightMan = world::Manager::getLightManager();
    
    if (args == "off") {
        lightMan->setAmbientIntensity(CL_Vec3f(0.8, 0.8, 0.8));
        lightMan->setGlobalIntensity(CL_Vec3f(0, 0, 0));
    } else if (args == "on") {
        lightMan->setAmbientIntensity(CL_Vec3f(0.65, 0.65, 0.65));
        lightMan->setGlobalIntensity(CL_Vec3f(0.5, 0.5, 0.5));
    }
}

}
}
}

