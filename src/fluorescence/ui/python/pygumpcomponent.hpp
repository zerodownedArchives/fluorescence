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

namespace python {

class PyGumpComponent {
public:
    static boost::python::tuple getGeometry(const GumpComponent* self);
    static void setGeometry(GumpComponent* self, boost::python::tuple& geom);

    static boost::python::tuple getRgba(const GumpComponent* self);
    static void setRgba(GumpComponent* self, boost::python::tuple& rgba);
    static void setRgba3(GumpComponent* self, float r, float g, float b);
    static void setRgba4(GumpComponent* self, float r, float g, float b, float a);
    static void setRgbaString(GumpComponent* self, const std::string& str);

};

}
}
}

#endif

