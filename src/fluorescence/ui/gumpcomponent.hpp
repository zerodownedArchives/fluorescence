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

    bool useRgba() const;
    void setUseRgba(bool value);

    unsigned int getHue() const;
    void setHue(unsigned int hue);

    bool usePartialHue() const;
    void setUsePartialHue(bool value);

    float getAlpha() const;
    void setAlpha(float alpha);

    CL_Colorf getRgba() const;
    void setRgba(const CL_Colorf& rgba);
    void setRgba(float r, float g, float b, float a);
    void setRgba(float r, float g, float b);

    CL_Vec3f getHueVector() const;

    virtual void setAutoResize(bool value);

protected:
    bool useRgba_;
    unsigned int hue_;
    bool usePartialHue_;
    float alpha_;
    CL_Colorf rgba_;
};

}
}

#endif
