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


#ifndef FLUO_UI_COMPONENTS_IMAGE_HPP
#define FLUO_UI_COMPONENTS_IMAGE_HPP

#include <ClanLib/Display/Render/graphic_context.h>
#include <ClanLib/Display/Render/texture.h>
#include <boost/shared_ptr.hpp>

#include <misc/string.hpp>
#include <ui/gumpcomponent.hpp>

namespace fluo {
namespace ui {

class Texture;
class XmlParser;

namespace components {

class Image;

class ImageState {

friend class Image;

public:
    ImageState();

    void setOwner(Image* owner);

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

private:
    Image* owner_;

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

class Image : public GumpComponent {

friend class ui::XmlParser;

public:
    Image(CL_GUIComponent* parent);

    void render(CL_GraphicContext& gc, const CL_Rect& clipRect);

    virtual void setAutoResize(bool value);

    virtual bool has_pixel(const CL_Point& p) const;

    void setCurrentState(const UnicodeString& name);
    UnicodeString getCurrentStateName() const;
    ImageState* getState(const UnicodeString& name);

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

    bool useRgba() const;

    bool getPartialHue() const;
    void setPartialHue(bool value);

    CL_Vec3f getHueVector() const;

    float getAlpha() const;
    void setAlpha(float alpha);

    CL_Colorf getFontRgba() const;
    void setFontRgba(const CL_Colorf& rgba);
    void setFontRgba(float r, float g, float b, float a);
    void setFontRgba(float r, float g, float b);

    UnicodeString getText() const;
    void setText(const UnicodeString& text);

private:
    bool autoResize_;

    void renderShader(CL_GraphicContext& gc, const CL_Rect& clipRect);

    std::map<UnicodeString, ImageState> states_;
    ImageState* defaultState_;
    ImageState* currentState_;
    UnicodeString currentStateName_;
};

}
}
}

#endif

