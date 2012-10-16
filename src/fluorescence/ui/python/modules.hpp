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
#include "pygumpcomponentcontainer.hpp"
#include "pydata.hpp"

#include <data/manager.hpp>

#include <ui/componentlist.hpp>

namespace fluo {
namespace ui {
namespace python {

namespace bpy = boost::python;

BOOST_PYTHON_MODULE(data) {
    // register shared pointer conversions
    bpy::register_ptr_to_python< boost::shared_ptr<ui::Texture> >();

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
    bpy::class_<ui::Texture, boost::noncopyable>("TextureWrapper", bpy::no_init)
        .def_readonly("ready", &ui::Texture::isReadComplete)
        .def_readonly("width", &ui::Texture::getWidth)
        .def_readonly("height", &ui::Texture::getHeight)
    ;

    // factory functions for textures
    bpy::def("Texture", &PyData::getTexture);
    bpy::def("Texture", &PyData::getTextureInt);


    // factory functions for cliloc strings
    bpy::def("cltest", PyData::test);
    bpy::def("Cliloc", PyData::cliloc);
    bpy::def("Cliloc", PyData::clilocArgs);
}


BOOST_PYTHON_MODULE(gumps) {
    // base class used for gump menus and scrollbars (i.e. components to which you can add other components)
    bpy::class_<CL_GUIComponent, boost::noncopyable>("GumpComponentContainer", bpy::no_init)
        .def("addImage", &PyGumpComponentContainer::addImage, bpy::return_value_policy<bpy::reference_existing_object>())
        .def("addBackground", &PyGumpComponentContainer::addBackground, bpy::return_value_policy<bpy::reference_existing_object>())
    ;

    // non-instanciable wrapper class for gump menus
    bpy::class_<GumpMenu, bpy::bases<CL_GUIComponent>, boost::noncopyable>("GumpMenuWrapper", bpy::no_init)
        //.def("setClosable", &GumpMenu::setClosable)
    ;

    // factory functions for gump menus
    bpy::def("GumpMenu", &PyGumpMenu::create, bpy::return_value_policy<bpy::reference_existing_object>());
    bpy::def("GumpMenu", &PyGumpMenu::createBackground, bpy::return_value_policy<bpy::reference_existing_object>());

    // base class for gump components
    bpy::class_<GumpComponent, boost::noncopyable>("GumpComponentWrapper", bpy::no_init)
        .add_property("x", &GumpComponent::getX, &GumpComponent::setX)
        .add_property("y", &GumpComponent::getY, &GumpComponent::setY)
        .add_property("width", &GumpComponent::getWidth, &GumpComponent::setWidth)
        .add_property("height", &GumpComponent::getHeight, &GumpComponent::setHeight)
        .add_property("geometry", &PyGumpComponent::getGeometry, &PyGumpComponent::setGeometry)
        .def("setGeometry", &GumpComponent::setGeometry)

        .add_property("useRgba", &GumpComponent::useRgba, &GumpComponent::setUseRgba)
        .add_property("hue", &GumpComponent::getHue, &GumpComponent::setHue)
        .add_property("alpha", &GumpComponent::getAlpha, &GumpComponent::setAlpha)
        .add_property("usePartialHue", &GumpComponent::usePartialHue, &GumpComponent::setUsePartialHue)
        .add_property("rgba", &PyGumpComponent::getRgba, &PyGumpComponent::setRgba)
        .def("setRgba", &PyGumpComponent::setRgba3)
        .def("setRgba", &PyGumpComponent::setRgba4)
        .def("setRgba", &PyGumpComponent::setRgbaString)
    ;

    // image component
    bpy::class_<components::Image, bpy::bases<GumpComponent>, boost::noncopyable>("ImageWrapper", bpy::no_init)
        .add_property("texture", &components::Image::getTexture, &components::Image::setTexture)
    ;


    // background component
    bpy::class_<components::Background, bpy::bases<GumpComponent>, boost::noncopyable>("BackgroundWrapper", bpy::no_init)
        .add_property("baseId", &components::Background::getBaseId, &components::Background::setBaseId)
    ;
}

}
}
}

#endif

