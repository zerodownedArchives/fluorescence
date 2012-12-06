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


#ifndef FLUO_UI_PYTHON_MODWORLD_HPP
#define FLUO_UI_PYTHON_MODWORLD_HPP

#include "pyworld.hpp"

#include <world/mobile.hpp>
#include <world/dynamicitem.hpp>

namespace fluo {
namespace ui {
namespace python {

namespace bpy = boost::python;

BOOST_PYTHON_MODULE(world) {
    bpy::class_<world::IngameObject, boost::shared_ptr<world::IngameObject>, boost::noncopyable>("IngameObject", bpy::no_init)
    ;

    bpy::class_<world::ServerObject, boost::shared_ptr<world::ServerObject>, bpy::bases<world::IngameObject>, boost::noncopyable>("ServerObject", bpy::no_init)
        .add_property("serial", &world::ServerObject::getSerial)
    ;

    bpy::class_<world::Mobile, boost::shared_ptr<world::Mobile>, bpy::bases<world::ServerObject>, boost::noncopyable>("Mobile", bpy::no_init)
        .add_property("warmode", &world::Mobile::isWarMode, &world::Mobile::setWarMode)
        .def("__eq__", &world::Mobile::equalWrap)
        .def("openStatus", &world::Mobile::openStatusGump)
        .def("openSkills", &world::Mobile::openSkillsGump)
        .def("openProfile", &PyWorld::openProfile)
        .def("getProperty", &world::Mobile::pyGetProperty)
    ;

    bpy::def("getPlayer", &PyWorld::getPlayer);
    bpy::def("getMobile", &PyWorld::getMobile);
    bpy::def("getDynamicItem", &PyWorld::getDynamicItem);

    bpy::class_<world::DynamicItem, boost::shared_ptr<world::DynamicItem>, bpy::bases<world::ServerObject>, boost::noncopyable>("DynamicItem", bpy::no_init)
        .def("setSpellbookGump", &world::DynamicItem::setSpellbookGump)
    ;
}

}
}
}

#endif

