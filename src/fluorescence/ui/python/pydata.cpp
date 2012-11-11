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

#include "pydata.hpp"

#include <data/manager.hpp>
#include <data/clilocloader.hpp>
#include <data/huesloader.hpp>

#include <boost/python/extract.hpp>

namespace fluo {
namespace ui {
namespace python {

UnicodeString PyData::cliloc(unsigned int id) {
    return data::Manager::getClilocLoader()->get(id);
}

UnicodeString PyData::clilocArgs(unsigned int id, boost::python::list args) {
    namespace bpy = boost::python;

    std::vector<UnicodeString> cppArgs;

    bpy::ssize_t n = bpy::len(args);
    for(bpy::ssize_t i=0; i<n; ++i) {
        cppArgs.push_back(bpy::extract<UnicodeString>(args[i]));
    }

    return data::Manager::getClilocLoader()->get(id, cppArgs);
}

boost::shared_ptr<ui::Texture> PyData::getTexture(unsigned int src, const UnicodeString& id) {
    return data::Manager::getTexture(src, id);
}

boost::shared_ptr<ui::Texture> PyData::getTextureInt(unsigned int src, unsigned int id) {
    return data::Manager::getTexture(src, id);
}

boost::python::tuple PyData::hueToRgba(unsigned int hue) {
    CL_Colorf rgba = data::Manager::getHuesLoader()->getFontClColor(hue);
    return boost::python::make_tuple(rgba.r, rgba.g, rgba.b, rgba.a);
}

boost::python::tuple PyData::rgba4(float r, float g, float b, float a) {
    return boost::python::make_tuple(r, g, b, a);
}

boost::python::tuple PyData::rgba3(float r, float g, float b) {
    return rgba4(r, g, b, 1.0);
}

boost::python::tuple PyData::rgbaString(const UnicodeString& str) {
    CL_Colorf rgba(StringConverter::toUtf8String(str));
    return boost::python::make_tuple(rgba.r, rgba.g, rgba.b, rgba.a);
}

}
}
}

