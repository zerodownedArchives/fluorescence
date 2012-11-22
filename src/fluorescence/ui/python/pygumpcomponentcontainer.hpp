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


#ifndef FLUO_UI_PYTHON_PYGUMPCOMPONENTCONTAINER_HPP
#define FLUO_UI_PYTHON_PYGUMPCOMPONENTCONTAINER_HPP

#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>

#include <misc/string.hpp>

class CL_GUIComponent;

namespace fluo {

namespace world {
class Mobile;
class DynamicItem;
}

namespace ui {

class Texture;

namespace components {
class Image;
class Background;
class AlphaRegion;
class Label;
class Button;
class LineEdit;
class ScrollArea;
class Checkbox;
class RadioButton;
class WorldView;
class PaperdollView;
class PropertyLabel;
class SysLogLabel;
class ContainerView;
}

namespace python {

class PyGumpComponentContainer {
public:
    static components::AlphaRegion* addAlphaRegion(CL_GUIComponent* self, boost::python::tuple& geom, float alpha);
    static components::Background* addBackground(CL_GUIComponent* self, boost::python::tuple& geom, unsigned int baseId);
    static components::Image* addImage(CL_GUIComponent* self, boost::python::tuple& geom, const boost::shared_ptr<ui::Texture>& tex);
    static components::Label* addLabel(CL_GUIComponent* self, boost::python::tuple& geom, const UnicodeString& text);
    static components::Button* addPageButton(CL_GUIComponent* self, boost::python::tuple& geom, const boost::shared_ptr<ui::Texture>& tex, unsigned int page);
    static components::Button* addServerButton(CL_GUIComponent* self, boost::python::tuple& geom, const boost::shared_ptr<ui::Texture>& tex, unsigned int id);
    static components::Button* addPythonButton(CL_GUIComponent* self, boost::python::tuple& geom, const boost::shared_ptr<ui::Texture>& tex, boost::python::object& func);
    static components::LineEdit* addLineEdit(CL_GUIComponent* self, boost::python::tuple& geom);
    static components::ScrollArea* addScrollArea(CL_GUIComponent* self, boost::python::tuple& geom);
    static components::Checkbox* addCheckbox(CL_GUIComponent* self, boost::python::tuple& geom);
    static components::RadioButton* addRadioButton(CL_GUIComponent* self, boost::python::tuple& geom, unsigned int group);
    static components::WorldView* addWorldView(CL_GUIComponent* self, boost::python::tuple& geom);
    static components::PaperdollView* addPaperdollView(CL_GUIComponent* self, boost::python::tuple& geom, const boost::shared_ptr<world::Mobile>& mob);
    static components::PropertyLabel* addPropertyLabel(CL_GUIComponent* self, boost::python::tuple& geom, const UnicodeString& prop);
    static components::SysLogLabel* addSysLogLabel(CL_GUIComponent* self, boost::python::tuple& geom);
    static components::Label* addHtmlLabel(CL_GUIComponent* self, boost::python::tuple& geom, const UnicodeString& html);
    static components::ContainerView* addContainerView(CL_GUIComponent* self, boost::python::tuple& geom, const boost::shared_ptr<world::DynamicItem>& itm);

private:
    // when adding to a scrollarea, the components are not added directly to the area but to a child component
    // this function checks if comp is a scrollbar, and returns the content component
    static CL_GUIComponent* getParentHelper(CL_GUIComponent* comp);
};

}
}
}

#endif

