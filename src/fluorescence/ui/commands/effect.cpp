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

#include <typedefs.hpp>
#include <world/manager.hpp>
#include <world/ingameobject.hpp>
#include <world/mobile.hpp>
#include <world/particleeffect.hpp>
#include <ui/particles/xmlloader.hpp>

namespace fluo {
namespace ui {
namespace commands {

Effect::Effect() : ClientCommand("Usage: effect <name>. Displays the given particle effect") {
}

void Effect::execute(const UnicodeString& args) {
    world::Manager* worldMan = world::Manager::getSingleton();

    boost::shared_ptr<world::IngameObject> sourceObj = boost::static_pointer_cast<world::IngameObject>(worldMan->getPlayer());

    boost::shared_ptr<world::Effect> effect = ui::particles::XmlLoader::fromFile(args);
    if (effect) {
        effect->setAtSource(sourceObj);
        effect->setShouldExplode(false);
        worldMan->addEffect(effect);
    }
}

}
}
}

