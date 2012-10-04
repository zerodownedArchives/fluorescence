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

#include "pystring.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace python {

namespace bpy = boost::python;


PythonUnicodeStringConverter::PythonUnicodeStringConverter() {
    bpy::to_python_converter<
        UnicodeString,
        PythonUnicodeStringConverter>();

    bpy::converter::registry::push_back(
        &isConvertible,
        &convertFromPython,
        bpy::type_id<UnicodeString>());
}

void* PythonUnicodeStringConverter::isConvertible(PyObject* obj) {
    //LOG_DEBUG << "pystr convertible" << std::endl;
    return (PyString_Check(obj) || PyUnicode_Check(obj)) ? obj : nullptr;
}

void PythonUnicodeStringConverter::convertFromPython(PyObject* obj, bpy::converter::rvalue_from_python_stage1_data* data) {
    //LOG_DEBUG << "pystr convert from python" << std::endl;
    if (PyString_Check(obj)) {
        const char* value = PyString_AsString(obj);
        void* storage = ((bpy::converter::rvalue_from_python_storage<UnicodeString>*)data)->storage.bytes;
        if (!value) {
            new (storage)UnicodeString();
        } else {
            new (storage)UnicodeString(value);
        }
        data->convertible = storage;
    } else if (PyUnicode_Check(obj)) {
        bpy::handle<> utf8(bpy::allow_null(PyUnicode_AsUTF8String(obj)));
        void* storage = ((bpy::converter::rvalue_from_python_storage<UnicodeString>*)data)->storage.bytes;
        if (!utf8) {
            LOG_ERROR << "Could not convert Python unicode object to UnicodeString" << std::endl;
            new (storage) UnicodeString();
        } else {
            const char* utf8v = PyString_AsString(utf8.get());
            if (!utf8v) {
                new (storage) UnicodeString();
            } else {
                new(storage) UnicodeString(utf8v);
            }
        }
        data->convertible = storage;
    } else {
        LOG_ERROR << "Unexpected type for Python string conversion" << std::endl;
    }
}

PyObject* PythonUnicodeStringConverter::convert(const UnicodeString& s) {
    //LOG_DEBUG << "pystr convert to python" << std::endl;
    return PyUnicode_FromString(StringConverter::toUtf8String(s).c_str());
}

}
}
}

