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


#ifndef FLUO_UI_PYTHON_PYDATA_HPP
#define FLUO_UI_PYTHON_PYDATA_HPP

#include <misc/string.hpp>

#include <boost/python/list.hpp>
#include <boost/python/tuple.hpp>

namespace fluo {
namespace ui {

class Texture;

namespace python {

class PyData {
public:
    static UnicodeString test();
    static UnicodeString cliloc(unsigned int id);
    static UnicodeString clilocArgs(unsigned int id, boost::python::list args);

    static boost::shared_ptr<ui::Texture> getTexture(unsigned int src, const UnicodeString& id);
    static boost::shared_ptr<ui::Texture> getTextureInt(unsigned int src, unsigned int id);

    static boost::python::tuple hueToRgba(unsigned int hue);

    static boost::python::tuple rgba3(float r, float g, float b);
    static boost::python::tuple rgba4(float r, float g, float b, float a);
    static boost::python::tuple rgbaString(const UnicodeString& str);

    static boost::python::list getSkillList();
};

}
}
}

#endif
