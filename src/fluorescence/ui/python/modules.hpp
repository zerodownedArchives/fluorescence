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

#include <ui/enums.hpp>
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
    bpy::def("Cliloc", PyData::cliloc);
    bpy::def("Cliloc", PyData::clilocArgs);

    // convert hue value to rgba tuple
    bpy::def("rgba", PyData::hueToRgba);
    // create proper rgba tuples
    bpy::def("rgba", PyData::rgba3);
    bpy::def("rgba", PyData::rgba4);
    bpy::def("rgba", PyData::rgbaString);
}


BOOST_PYTHON_MODULE(gumps) {
    // alignment for labels, etc
    bpy::enum_<ui::HAlign>("HAlign")
        .value("LEFT", ui::HAlign::LEFT)
        .value("RIGHT", ui::HAlign::RIGHT)
        .value("CENTER", ui::HAlign::CENTER)
    ;

    bpy::enum_<ui::VAlign>("VAlign")
        .value("TOP", ui::VAlign::TOP)
        .value("BOTTOM", ui::VAlign::BOTTOM)
        .value("MIDDLE", ui::VAlign::MIDDLE)
    ;

    // base class used for gump menus and scrollbars (i.e. components to which you can add other components)
    bpy::class_<CL_GUIComponent, boost::noncopyable>("GumpComponentContainer", bpy::no_init)
        .def("addImage", &PyGumpComponentContainer::addImage, bpy::return_value_policy<bpy::reference_existing_object>())
        .def("addBackground", &PyGumpComponentContainer::addBackground, bpy::return_value_policy<bpy::reference_existing_object>())
        .def("addAlphaRegion", &PyGumpComponentContainer::addAlphaRegion, bpy::return_value_policy<bpy::reference_existing_object>())
        .def("addLabel", &PyGumpComponentContainer::addLabel, bpy::return_value_policy<bpy::reference_existing_object>())
        .def("addButton", &PyGumpComponentContainer::addButton, bpy::return_value_policy<bpy::reference_existing_object>())
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
    ;

    // image component
    bpy::class_<components::Image, bpy::bases<GumpComponent>, boost::noncopyable>("ImageWrapper", bpy::no_init)
        // the functions in this block are just forwarded to the current ImageState
        .add_property("texture", &components::Image::getTexture, &components::Image::setTexture)
        .add_property("hue", &components::Image::getHue, &components::Image::setHue)
        .add_property("alpha", &components::Image::getAlpha, &components::Image::setAlpha)
        .add_property("usePartialHue", &components::Image::getPartialHue, &components::Image::setPartialHue)
        .add_property("tiled", &components::Image::getTiled, &components::Image::setTiled)
        .add_property("text", &components::Image::getText, &components::Image::setText)
        .add_property("rgba", &PyGumpComponent::getRgbaImage, &PyGumpComponent::setRgbaImage)
        .add_property("fontRgba", &PyGumpComponent::getFontRgbaImage, &PyGumpComponent::setFontRgbaImage)

        // these functions are really part of the image class
        .add_property("valign", &components::Image::getVAlign, &components::Image::setVAlign)
        .add_property("halign", &components::Image::getHAlign, &components::Image::setHAlign)
        .def("state", &components::Image::getState, bpy::return_value_policy<bpy::reference_existing_object>())
        .def("setState", &components::Image::setCurrentState)
        .def("setFont", &components::Image::setFont)
    ;

    // image states
    bpy::class_<components::ImageState, boost::noncopyable>("ImageStateWrapper", bpy::no_init)
        .add_property("texture", &components::ImageState::getTexture, &components::ImageState::setTexture)
        .add_property("hue", &components::ImageState::getHue, &components::ImageState::setHue)
        .add_property("alpha", &components::ImageState::getAlpha, &components::ImageState::setAlpha)
        .add_property("usePartialHue", &components::ImageState::getPartialHue, &components::ImageState::setPartialHue)
        .add_property("tiled", &components::ImageState::getTiled, &components::ImageState::setTiled)
        .add_property("text", &components::ImageState::getText, &components::ImageState::setText)
        .add_property("rgba", &PyGumpComponent::getRgbaImageState, &PyGumpComponent::setRgbaImageState)
        .add_property("fontRgba", &PyGumpComponent::getFontRgbaImageState, &PyGumpComponent::setFontRgbaImageState)
    ;

    // button component
    bpy::class_<components::Button, bpy::bases<components::Image>, boost::noncopyable>("ButtonWrapper", bpy::no_init)
        .add_property("mouseover", make_function(&components::Button::getStateMouseOver, bpy::return_value_policy<bpy::reference_existing_object>()))
        .add_property("mousedown", make_function(&components::Button::getStateMouseDown, bpy::return_value_policy<bpy::reference_existing_object>()))
    ;

    // background component
    bpy::class_<components::Background, bpy::bases<GumpComponent>, boost::noncopyable>("BackgroundWrapper", bpy::no_init)
        .add_property("baseId", &components::Background::getBaseId, &components::Background::setBaseId)
        .add_property("hue", &components::Image::getHue, &components::Image::setHue)
        .add_property("alpha", &components::Image::getAlpha, &components::Image::setAlpha)
        .add_property("usePartialHue", &components::Image::getPartialHue, &components::Image::setPartialHue)
        .add_property("rgba", &PyGumpComponent::getRgbaBackground, &PyGumpComponent::setRgbaBackground)
    ;

    // alpha region component
    bpy::class_<components::AlphaRegion, bpy::bases<GumpComponent>, boost::noncopyable>("AlphaRegionWrapper", bpy::no_init)
        .add_property("alpha", &components::AlphaRegion::getAlpha, &components::AlphaRegion::setAlpha)
    ;

    // label component
    bpy::class_<components::Label, bpy::bases<GumpComponent>, boost::noncopyable>("LabelWrapper", bpy::no_init)
        .add_property("text", &components::Label::getText, &components::Label::setText)
        .add_property("align", &components::Label::getAlignment, &components::Label::setAlignment)
    ;
}

}
}
}

#endif

