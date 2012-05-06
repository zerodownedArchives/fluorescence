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

#include "osieffect.hpp"

#include <typedefs.hpp>
#include <world/manager.hpp>
#include <world/ingameobject.hpp>
#include <world/mobile.hpp>
#include <world/osieffect.hpp>

namespace fluo {
namespace ui {
namespace commands {
    
void OsiEffect::execute(const UnicodeString& args) {
    int effectId = StringConverter::toInt(args);
    if (effectId == 0) {
        return;
    }
    
    world::Manager* worldMan = world::Manager::getSingleton();
    
    boost::shared_ptr<world::IngameObject> sourceObj = boost::static_pointer_cast<world::IngameObject>(worldMan->getPlayer());
    
    boost::shared_ptr<world::Effect> effect(new world::OsiEffect(effectId));
    effect->setAtSource(sourceObj);
    effect->setLifetimeMillis(10 * 50);
    effect->setShouldExplode(false);
    worldMan->addEffect(effect);
}

}
}
}

