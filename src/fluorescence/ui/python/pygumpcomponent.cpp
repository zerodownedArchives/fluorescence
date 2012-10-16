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

#include "pygumpcomponent.hpp"

#include <boost/python/extract.hpp>

#include <ui/gumpcomponent.hpp>

namespace fluo {
namespace ui {
namespace python {

boost::python::tuple PyGumpComponent::getGeometry(const GumpComponent* self) {
    CL_Rectf geom = self->getGeometry();
    return boost::python::make_tuple(geom.left, geom.top, geom.get_width(), geom.get_height());
}

void PyGumpComponent::setGeometry(GumpComponent* self, boost::python::tuple& geom) {
    namespace bpy = boost::python;

    int x = bpy::extract<int>(geom[0]);
    int y = bpy::extract<int>(geom[1]);

    int width = 1;
    int height = 1;
    if (bpy::len(geom) > 3) {
        width = bpy::extract<int>(geom[2]);
        height = bpy::extract<int>(geom[3]);

        self->setAutoResize(false);
    } else {
        self->setAutoResize(true);
    }

    CL_Rectf rect(x, y, CL_Sizef(width, height));
    self->set_geometry(rect);
}

boost::python::tuple PyGumpComponent::getRgba(const GumpComponent* self) {
    CL_Colorf rgba = self->getRgba();
    return boost::python::make_tuple(rgba.r, rgba.g, rgba.b, rgba.a);
}

void PyGumpComponent::setRgba(GumpComponent* self, boost::python::tuple& rgba) {
    namespace bpy = boost::python;

    float r = bpy::extract<float>(rgba[0]);
    float g = bpy::extract<float>(rgba[1]);
    float b = bpy::extract<float>(rgba[2]);

    if (bpy::len(rgba) > 3) {
        float a = bpy::extract<float>(rgba[3]);
        self->setRgba(r, g, b, a);
    } else {
        self->setRgba(r, g, b);
    }
}

void PyGumpComponent::setRgba4(GumpComponent* self, float r, float g, float b, float a) {
    self->setRgba(r, g, b, a);
}

void PyGumpComponent::setRgba3(GumpComponent* self, float r, float g, float b) {
    self->setRgba(r, g, b);
}

void PyGumpComponent::setRgbaString(GumpComponent* self, const std::string& str) {
    CL_Colorf col(str);
    self->setRgba(col);
}

}
}
}

