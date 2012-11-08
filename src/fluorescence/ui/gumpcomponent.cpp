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



#include "gumpcomponent.hpp"

#include "gumpmenu.hpp"

#include <boost/python/extract.hpp>

namespace fluo {
namespace ui {

GumpComponent::GumpComponent(CL_GUIComponent* parent) :
        CL_GUIComponent(parent) {
    set_double_click_enabled(false);
}

int GumpComponent::getX() const {
    return get_geometry().left;
}

void GumpComponent::setX(int x) {
    CL_Rectf geom = get_geometry();
    int diff = x - geom.left;
    geom.translate(diff, 0);
    set_geometry(geom);
}

int GumpComponent::getY() const {
    return get_geometry().top;
}

void GumpComponent::setY(int y) {
    CL_Rectf geom = get_geometry();
    int diff = y - geom.top;
    geom.translate(0, diff);
    set_geometry(geom);
}

int GumpComponent::getWidth() const {
    return get_geometry().get_width();
}

void GumpComponent::setWidth(int w) {
    CL_Rectf geom = get_geometry();
    geom.set_width(w);
    set_geometry(geom);
}

int GumpComponent::getHeight() const {
    return get_geometry().get_height();
}

void GumpComponent::setHeight(int h) {
    CL_Rectf geom = get_geometry();
    geom.set_height(h);
    set_geometry(geom);
}

CL_Rectf GumpComponent::getGeometry() const {
    return get_geometry();
}

void GumpComponent::setGeometry(int x, int y, int w, int h) {
    CL_Rectf geom(x, y, CL_Sizef(w, h));
    set_geometry(geom);
}

void GumpComponent::setAutoResize(bool value) {
}

boost::python::tuple GumpComponent::pyGetGeometry() const {
    CL_Rectf geom = get_geometry();
    return boost::python::make_tuple(geom.left, geom.top, geom.get_width(), geom.get_height());
}

void GumpComponent::pySetGeometry(boost::python::tuple& geom) {
    namespace bpy = boost::python;

    int x = bpy::extract<int>(geom[0]);
    int y = bpy::extract<int>(geom[1]);

    int width = 1;
    int height = 1;
    if (bpy::len(geom) > 3) {
        width = bpy::extract<int>(geom[2]);
        height = bpy::extract<int>(geom[3]);

        setAutoResize(false);
    } else {
        setAutoResize(true);
    }

    CL_Rectf rect(x, y, CL_Sizef(width, height));
    set_geometry(rect);
}

const GumpMenu* GumpComponent::getGumpMenu() const {
    const CL_GUIComponent* topLevel = get_top_level_component();
    return dynamic_cast<const GumpMenu*>(topLevel);
}

GumpMenu* GumpComponent::getGumpMenu() {
    CL_GUIComponent* topLevel = get_top_level_component();
    return dynamic_cast<GumpMenu*>(topLevel);
}

UnicodeString GumpComponent::getName() const {
    return StringConverter::fromUtf8(get_id_name().c_str());
}

void GumpComponent::setName(const UnicodeString& name) {
    set_id_name(StringConverter::toUtf8String(name));
}

GumpMenu* GumpComponent::pyGetGumpMenu() {
    return getGumpMenu();
}

boost::python::dict GumpComponent::getPythonStore() const {
    return pythonStore_;
}

void GumpComponent::focus() {
    set_focus(true);
}

}
}
