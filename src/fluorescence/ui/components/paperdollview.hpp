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


#ifndef FLUO_UI_PAPERDOLLVIEW_HPP
#define FLUO_UI_PAPERDOLLVIEW_HPP

#include <boost/shared_ptr.hpp>

#include <list>

#include <ui/gumpelement.hpp>

namespace fluo {

namespace world {
class IngameObject;
class Mobile;
}

namespace ui {

class PaperdollRenderer;

namespace components {

class PaperdollView : public GumpElement {
public:
    PaperdollView(CL_GUIComponent* parent, const CL_Rect& bounds);
    ~PaperdollView();

    unsigned int getWidth();
    unsigned int getHeight();

    void renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect);

    void addObject(boost::shared_ptr<world::IngameObject> obj);
    void removeObject(boost::shared_ptr<world::IngameObject> obj);

    virtual bool has_pixel(const CL_Point& p) const;

    void setMobile(boost::shared_ptr<world::Mobile> mob);

private:
    boost::shared_ptr<PaperdollRenderer> renderer_;

    bool onInputPressed(const CL_InputEvent& e);
    bool onInputReleased(const CL_InputEvent & e);
    bool onDoubleClick(const CL_InputEvent& e);

    boost::shared_ptr<world::Mobile> mobile_;
};

}
}
}

#endif
