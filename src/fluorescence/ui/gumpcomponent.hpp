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


#ifndef FLUO_UI_GUMPCOMPONENT_HPP
#define FLUO_UI_GUMPCOMPONENT_HPP

#include <ClanLib/GUI/gui_component.h>
#include <boost/python/tuple.hpp>

#include <misc/string.hpp>

namespace fluo {
namespace ui {

class GumpMenu;

class GumpComponent : public CL_GUIComponent {
public:
    GumpComponent(CL_GUIComponent* parent);

    int getX() const;
    void setX(int x);

    int getY() const;
    void setY(int y);

    int getWidth() const;
    void setWidth(int w);

    int getHeight() const;
    void setHeight(int h);

    CL_Rectf getGeometry() const;
    void setGeometry(int x, int y, int w, int h);

    virtual void setAutoResize(bool value);

    GumpMenu* getGumpMenu();

    UnicodeString getName() const;
    void setName(const UnicodeString& name);

    // python specific functions
    boost::python::tuple pyGetGeometry() const;
    void pySetGeometry(boost::python::tuple& geom);
};

}
}

#endif
