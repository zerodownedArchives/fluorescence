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

#include "pygumpcomponentcontainer.hpp"
#include "pydata.hpp"
#include "pyclient.hpp"

#include <data/manager.hpp>

#include <ui/gumpmenu.hpp>
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


BOOST_PYTHON_MODULE(client) {
    bpy::def("connect", PyClient::connect);
    bpy::def("getConfig", PyClient::getConfig);
    bpy::def("shutdown", PyClient::shutdown);
    bpy::def("messagebox", PyClient::messagebox);
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
        .def("addPageButton", &PyGumpComponentContainer::addPageButton, bpy::return_value_policy<bpy::reference_existing_object>())
        .def("addServerButton", &PyGumpComponentContainer::addServerButton, bpy::return_value_policy<bpy::reference_existing_object>())
        .def("addPythonButton", &PyGumpComponentContainer::addPythonButton, bpy::return_value_policy<bpy::reference_existing_object>())
        .def("addLineEdit", &PyGumpComponentContainer::addLineEdit, bpy::return_value_policy<bpy::reference_existing_object>())
    ;

    // non-instanciable wrapper class for gump menus
    bpy::class_<GumpMenu, bpy::bases<CL_GUIComponent>, boost::noncopyable>("GumpMenuWrapper", bpy::no_init)
        .add_property("closable", &GumpMenu::isClosable, &GumpMenu::setClosable)
        .add_property("draggable", &GumpMenu::isDraggable, &GumpMenu::setDraggable)
        .add_property("store", &GumpMenu::getPythonStore)
        .add_property("onEnter", &GumpMenu::getPyEnterCallback, &GumpMenu::setPyEnterCallback)
        .add_property("onEscape", &GumpMenu::getPyEscapeCallback, &GumpMenu::setPyEscapeCallback)
        .def("setFont", &GumpMenu::setFont)
        .def("setFont", &GumpMenu::setFontB)
        .def("component", &GumpMenu::getNamedComponent, bpy::return_value_policy<bpy::reference_existing_object>())
        .def("close", &GumpMenu::close)
    ;

    // factory functions for gump menus
    bpy::def("GumpMenu", &GumpMenu::create, bpy::return_value_policy<bpy::reference_existing_object>());
    bpy::def("GumpMenu", &GumpMenu::createBackground, bpy::return_value_policy<bpy::reference_existing_object>());

    // base class for gump components
    bpy::class_<GumpComponent, boost::noncopyable>("GumpComponentWrapper", bpy::no_init)
        .add_property("x", &GumpComponent::getX, &GumpComponent::setX)
        .add_property("y", &GumpComponent::getY, &GumpComponent::setY)
        .add_property("width", &GumpComponent::getWidth, &GumpComponent::setWidth)
        .add_property("height", &GumpComponent::getHeight, &GumpComponent::setHeight)
        .add_property("geometry", &GumpComponent::pyGetGeometry, &GumpComponent::pySetGeometry)
        .add_property("gump", bpy::make_function(&GumpComponent::pyGetGumpMenu, bpy::return_value_policy<bpy::reference_existing_object>()))
        .add_property("name", &GumpComponent::getName, &GumpComponent::setName)
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
        .add_property("rgba", &components::Image::pyGetRgba, &components::Image::pySetRgba)
        .add_property("fontRgba", &components::Image::pyGetFontRgba, &components::Image::pySetFontRgba)

        // these functions are really part of the image class
        .add_property("valign", &components::Image::getVAlign, &components::Image::setVAlign)
        .add_property("halign", &components::Image::getHAlign, &components::Image::setHAlign)
        .def("state", &components::Image::getState, bpy::return_value_policy<bpy::reference_existing_object>())
        .def("setState", &components::Image::setCurrentState)
        .def("setFont", &components::Image::setFont)
        .def("setFont", &components::Image::setFontB)
    ;

    // image states
    bpy::class_<components::ImageState, boost::noncopyable>("ImageStateWrapper", bpy::no_init)
        .add_property("texture", &components::ImageState::getTexture, &components::ImageState::setTexture)
        .add_property("hue", &components::ImageState::getHue, &components::ImageState::setHue)
        .add_property("alpha", &components::ImageState::getAlpha, &components::ImageState::setAlpha)
        .add_property("usePartialHue", &components::ImageState::getPartialHue, &components::ImageState::setPartialHue)
        .add_property("tiled", &components::ImageState::getTiled, &components::ImageState::setTiled)
        .add_property("text", &components::ImageState::getText, &components::ImageState::setText)
        .add_property("rgba", &components::ImageState::pyGetRgba, &components::ImageState::pySetRgba)
        .add_property("fontRgba", &components::ImageState::pyGetFontRgba, &components::ImageState::pySetFontRgba)
    ;

    // button component
    bpy::class_<components::Button, bpy::bases<components::Image>, boost::noncopyable>("ButtonWrapper", bpy::no_init)
        .add_property("mouseOver", make_function(&components::Button::getStateMouseOver, bpy::return_value_policy<bpy::reference_existing_object>()))
        .add_property("mouseDown", make_function(&components::Button::getStateMouseDown, bpy::return_value_policy<bpy::reference_existing_object>()))
        .add_property("page", &components::Button::getPage, &components::Button::setPage)
        .add_property("id", &components::Button::getButtonId, &components::Button::setButtonId)
        .add_property("onClick", &components::Button::getPyClickCallback, &components::Button::setPyClickCallback)
    ;

    // background component
    bpy::class_<components::Background, bpy::bases<GumpComponent>, boost::noncopyable>("BackgroundWrapper", bpy::no_init)
        .add_property("baseId", &components::Background::getBaseId, &components::Background::setBaseId)
        .add_property("hue", &components::Background::getHue, &components::Background::setHue)
        .add_property("alpha", &components::Background::getAlpha, &components::Background::setAlpha)
        .add_property("usePartialHue", &components::Background::getPartialHue, &components::Background::setPartialHue)
        .add_property("rgba", &components::Background::pyGetRgba, &components::Background::pySetRgba)
    ;

    // alpha region component
    bpy::class_<components::AlphaRegion, bpy::bases<GumpComponent>, boost::noncopyable>("AlphaRegionWrapper", bpy::no_init)
        .add_property("alpha", &components::AlphaRegion::getAlpha, &components::AlphaRegion::setAlpha)
    ;

    // label component
    bpy::class_<components::Label, bpy::bases<GumpComponent>, boost::noncopyable>("LabelWrapper", bpy::no_init)
        .add_property("text", &components::Label::getText, &components::Label::setText)
        .add_property("valign", &components::Label::getVAlign, &components::Label::setVAlign)
        .add_property("halign", &components::Label::getHAlign, &components::Label::setHAlign)
        .add_property("rgba", &components::Label::pyGetRgba, &components::Label::pySetRgba)
        .def("setFont", &components::Label::setFont)
        .def("setFont", &components::Label::setFontB)
    ;

    // lineedit component
    bpy::class_<components::LineEdit, bpy::bases<GumpComponent>, boost::noncopyable>("LineEditWrapper", bpy::no_init)
        .add_property("text", &components::LineEdit::getText, &components::LineEdit::setText)
        .add_property("rgba", &components::LineEdit::pyGetRgba, &components::LineEdit::pySetRgba)
        .add_property("numeric", &components::LineEdit::getNumericMode, &components::LineEdit::setNumericMode)
        .add_property("password", &components::LineEdit::getPasswordMode, &components::LineEdit::setPasswordMode)
        .add_property("id", &components::LineEdit::getEntryId, &components::LineEdit::setEntryId)
        .add_property("onEnter", &components::LineEdit::getPyEnterCallback, &components::LineEdit::setPyEnterCallback)
        .def("setFont", &components::LineEdit::setFont)
        .def("setFont", &components::LineEdit::setFontB)
    ;
}

}
}
}

#endif

