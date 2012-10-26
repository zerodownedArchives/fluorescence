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


#ifndef FLUO_UI_PYTHON_PYGUMPCOMPONENT_HPP
#define FLUO_UI_PYTHON_PYGUMPCOMPONENT_HPP

#include <boost/python/tuple.hpp>

namespace fluo {
namespace ui {

class GumpComponent;

namespace components {
class ImageState;
class Image;
class Background;
}

namespace python {

class PyGumpComponent {
public:
    static boost::python::tuple getGeometry(const GumpComponent* self);
    static void setGeometry(GumpComponent* self, boost::python::tuple& geom);

    static boost::python::tuple getRgbaImage(const components::Image* self);
    static void setRgbaImage(components::Image* self, boost::python::tuple& rgba);
    static boost::python::tuple getFontRgbaImage(const components::Image* self);
    static void setFontRgbaImage(components::Image* self, boost::python::tuple& rgba);

    static boost::python::tuple getRgbaImageState(const components::ImageState* self);
    static void setRgbaImageState(components::ImageState* self, boost::python::tuple& rgba);
    static boost::python::tuple getFontRgbaImageState(const components::ImageState* self);
    static void setFontRgbaImageState(components::ImageState* self, boost::python::tuple& rgba);

    static boost::python::tuple getRgbaBackground(const components::Background* self);
    static void setRgbaBackground(components::Background* self, boost::python::tuple& rgba);
};

}
}
}

#endif

