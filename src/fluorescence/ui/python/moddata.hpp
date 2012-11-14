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


#ifndef FLUO_UI_PYTHON_MODDATA_HPP
#define FLUO_UI_PYTHON_MODDATA_HPP

#include <boost/python.hpp>

#include "pydata.hpp"
#include <data/manager.hpp>

namespace fluo {
namespace ui {
namespace python {

namespace bpy = boost::python;

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

    // non-instanciable wrapper class for textures
    bpy::class_<ui::Texture, boost::shared_ptr<ui::Texture>, boost::noncopyable>("Texture", bpy::no_init)
        .def_readonly("ready", &ui::Texture::isReadComplete)
        .def_readonly("width", &ui::Texture::getWidth)
        .def_readonly("height", &ui::Texture::getHeight)
    ;

    // factory functions for textures
    bpy::def("Texture", &PyData::getTexture);
    bpy::def("Texture", &PyData::getTextureInt);


    // factory functions for cliloc strings
    bpy::def("Cliloc", &PyData::cliloc);
    bpy::def("Cliloc", &PyData::clilocArgs);

    // convert hue value to rgba tuple
    bpy::def("rgba", &PyData::hueToRgba);
    // create proper rgba tuples
    bpy::def("rgba", &PyData::rgba3);
    bpy::def("rgba", &PyData::rgba4);
    bpy::def("rgba", &PyData::rgbaString);

    bpy::def("getSkillList", &PyData::getSkillList);
}

}
}
}

#endif

