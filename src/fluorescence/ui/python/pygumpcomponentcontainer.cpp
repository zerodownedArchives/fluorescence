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

#include "pygumpcomponentcontainer.hpp"

#include "pygumpcomponent.hpp"

#include <misc/log.hpp>

#include <ui/gumpmenu.hpp>
#include <ui/componentlist.hpp>

namespace fluo {
namespace ui {
namespace python {

namespace bpy = boost::python;

components::Image* PyGumpComponentContainer::addImage(CL_GUIComponent* self, boost::python::tuple& geom, boost::shared_ptr<ui::Texture> tex) {
    components::Image* img = new components::Image(self);
    PyGumpComponent::setGeometry(img, geom);
    img->setTexture(tex);

    static_cast<GumpMenu*>(self->get_top_level_component())->addToCurrentPage(img);

    return img;
}

components::Background* PyGumpComponentContainer::addBackground(CL_GUIComponent* self, boost::python::tuple& geom, unsigned int baseId) {
    components::Background* bg = new components::Background(self);
    PyGumpComponent::setGeometry(bg, geom);
    bg->setBaseId(baseId);

    static_cast<GumpMenu*>(self->get_top_level_component())->addToCurrentPage(bg);

    return bg;
}

components::AlphaRegion* PyGumpComponentContainer::addAlphaRegion(CL_GUIComponent* self, boost::python::tuple& geom, float alpha) {
    components::AlphaRegion* region = new components::AlphaRegion(self);
    PyGumpComponent::setGeometry(region, geom);
    region->setAlpha(alpha);

    static_cast<GumpMenu*>(self->get_top_level_component())->addToCurrentPage(region);

    return region;
}

components::Label* PyGumpComponentContainer::addLabel(CL_GUIComponent* self, boost::python::tuple& geom, const UnicodeString& text) {
    components::Label* label = new components::Label(self);
    PyGumpComponent::setGeometry(label, geom);
    label->setText(text);

    static_cast<GumpMenu*>(self->get_top_level_component())->addToCurrentPage(label);

    return label;
}

components::Button* PyGumpComponentContainer::addButton(CL_GUIComponent* self, boost::python::tuple& geom, boost::shared_ptr<ui::Texture> tex) {
    components::Button* but = new components::Button(self);
    PyGumpComponent::setGeometry(but, geom);
    but->setTexture(tex);

    static_cast<GumpMenu*>(self->get_top_level_component())->addToCurrentPage(but);

    return but;
}

}
}
}

