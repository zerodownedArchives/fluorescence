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


#ifndef FLUO_UI_PYTHON_PYSTRING_HPP
#define FLUO_UI_PYTHON_PYSTRING_HPP

#include <misc/string.hpp>

#include <boost/python.hpp>

namespace fluo {
namespace ui {
namespace python {

struct PythonUnicodeStringConverter {
    PythonUnicodeStringConverter();

    // check if this python object is convertible to UnicodeString
    static void* isConvertible(PyObject* obj);
    // convert python object to UnicodeString
    static void convertFromPython(PyObject* obj, boost::python::converter::rvalue_from_python_stage1_data* data);

    // convert UnicodeString to python
    static PyObject* convert(const UnicodeString& s);
};

}
}
}

#endif

