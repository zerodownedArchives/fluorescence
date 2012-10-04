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


#ifndef FLUO_UI_PYTHON_MODULES_HPP
#define FLUO_UI_PYTHON_MODULES_HPP


#include "pygumpmenu.hpp"
#include "pygumpcomponent.hpp"

#include <data/manager.hpp>
#include "pytexture.hpp"
#include "pydata.hpp"

namespace fluo {
namespace ui {
namespace python {

namespace bpy = boost::python;

BOOST_PYTHON_MODULE(gumps) {
    bpy::class_<PyGumpMenu>("GumpMenu", bpy::no_init)
        .def(bpy::init<int, int>())
        .def(bpy::init<int, int, bool>())

        .def("addImage", &PyGumpMenu::addImage)

        //.def("setClosable", &GumpMenu::setClosable)
    ;

    bpy::class_<PyGumpComponent, boost::noncopyable>("GumpComponent", bpy::no_init)
    ;
}

BOOST_PYTHON_MODULE(data) {
    bpy::enum_<data::TextureSource::TextureSourceEnum>("TextureSource")
        .value("FILE", data::TextureSource::FILE)
        .value("MAPART", data::TextureSource::MAPART)
        .value("STATICART", data::TextureSource::STATICART)
        .value("GUMPART", data::TextureSource::GUMPART)
        .value("HTTP", data::TextureSource::HTTP)
        .value("THEME", data::TextureSource::THEME)
        .value("MAPTEX", data::TextureSource::MAPTEX)
    ;

    bpy::class_<PyTexture>("Texture", bpy::no_init)
        .def(bpy::init<unsigned int, const char*>())
    ;

    bpy::def("cltest", PyData::test);
    bpy::def("cliloc", PyData::cliloc);
    bpy::def("cliloc", PyData::clilocArgs);
    //bpy::def("clilocArgs", PyData::clilocArgs);
}

}
}
}

#endif

