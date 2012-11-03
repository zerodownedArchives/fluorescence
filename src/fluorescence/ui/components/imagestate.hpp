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


#ifndef FLUO_UI_COMPONENTS_IMAGESTATE_HPP
#define FLUO_UI_COMPONENTS_IMAGESTATE_HPP

#include <boost/shared_ptr.hpp>
#include <boost/python/tuple.hpp>
#include <ClanLib/Display/Render/texture.h>
#include <misc/string.hpp>

namespace fluo {
namespace ui {

class Texture;
class GumpComponent;

namespace components {

class Image;
class ScrollBar;

class ImageState {

friend class Image;
friend class ScrollBar;

public:
    ImageState();

    void setOwner(GumpComponent* owner);

    boost::shared_ptr<ui::Texture> getTexture() const;
    void setTexture(boost::shared_ptr<ui::Texture> tex);
    CL_Texture getTileableTexture();

    unsigned int getHue() const;
    void setHue(unsigned int hue);

    bool getTiled() const;
    void setTiled(bool value);

    CL_Colorf getRgba() const;
    void setRgba(const CL_Colorf& rgba);
    void setRgba(float r, float g, float b, float a);
    void setRgba(float r, float g, float b);

    bool getPartialHue() const;
    void setPartialHue(bool value);

    float getAlpha() const;
    void setAlpha(float alpha);

    CL_Colorf getFontRgba() const;
    void setFontRgba(const CL_Colorf& rgba);
    void setFontRgba(float r, float g, float b, float a);
    void setFontRgba(float r, float g, float b);

    UnicodeString getText() const;
    void setText(const UnicodeString& text);

    // python specific interface
    boost::python::tuple pyGetRgba() const;
    void pySetRgba(const boost::python::tuple& rgba);
    boost::python::tuple pyGetFontRgba() const;
    void pySetFontRgba(const boost::python::tuple& rgba);

private:
    GumpComponent* owner_;

    bool overrideTexture_;
    boost::shared_ptr<ui::Texture> texture_;

    bool overrideHue_;
    unsigned int hue_;

    bool overrideRgba_;
    CL_Colorf rgba_;

    bool overrideAlpha_;
    float alpha_;

    bool overrideFontRgba_;
    CL_Colorf fontRgba_;

    bool overrideText_;
    UnicodeString text_;

    bool overrideTiled_;
    bool tiled_;

    CL_Texture tileableTexture_;

    bool overridePartialHue_;
    bool partialHue_;
};

}
}
}

#endif

