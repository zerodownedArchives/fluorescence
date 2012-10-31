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

#include "image.hpp"

#include <ClanLib/Display/2D/draw.h>
#include <ClanLib/Core/Math/quad.h>
#include <ClanLib/Core/Math/rect.h>
#include <ClanLib/Display/Render/program_object.h>
#include <ClanLib/Display/2D/span_layout.h>
#include <boost/python/extract.hpp>

#include <ui/manager.hpp>
#include <ui/render/shadermanager.hpp>
#include <ui/texture.hpp>
#include <ui/gumpmenu.hpp>
#include "scrollarea.hpp"
#include <data/manager.hpp>
#include <data/huesloader.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace components {

ImageState::ImageState() :
        owner_(nullptr),
        overrideTexture_(false),
        overrideHue_(false), hue_(0),
        overrideRgba_(false), rgba_(CL_Colorf::white),
        overrideAlpha_(false), alpha_(1),
        overrideFontRgba_(false), fontRgba_(CL_Colorf::white),
        overrideText_(false),
        overrideTiled_(false), tiled_(false),
        overridePartialHue_(false), partialHue_(false) {
}

void ImageState::setOwner(Image* owner) {
    owner_ = owner;
}

void ImageState::setTexture(boost::shared_ptr<ui::Texture> texture) {
    overrideTexture_ = true;
    texture_ = texture;
    tileableTexture_ = CL_Texture();

    owner_->request_repaint();
}

boost::shared_ptr<ui::Texture> ImageState::getTexture() const {
    return texture_;
}

void ImageState::setHue(unsigned int hue) {
    overrideHue_ = true;
    hue_ = hue;

    owner_->request_repaint();
}

unsigned int ImageState::getHue() const {
    return hue_;
}

void ImageState::setRgba(const CL_Colorf& rgba) {
    overrideRgba_ = true;
    rgba_ = rgba;

    owner_->request_repaint();
}

void ImageState::setRgba(float r, float g, float b, float a) {
    setRgba(CL_Colorf(r, g, b, a));
}

void ImageState::setRgba(float r, float g, float b) {
    setRgba(CL_Colorf(r, g, b));
}

CL_Colorf ImageState::getRgba() const {
    return rgba_;
}

void ImageState::setAlpha(float alpha) {
    overrideAlpha_ = true;
    alpha_ = alpha;

    owner_->request_repaint();
}

float ImageState::getAlpha() const {
    return alpha_;
}

void ImageState::setFontRgba(const CL_Colorf& rgba) {
    overrideFontRgba_ = true;
    fontRgba_ = rgba;

    owner_->request_repaint();
}

void ImageState::setFontRgba(float r, float g, float b, float a) {
    setFontRgba(CL_Colorf(r, g, b, a));
}

void ImageState::setFontRgba(float r, float g, float b) {
    setFontRgba(CL_Colorf(r, g, b));
}

CL_Colorf ImageState::getFontRgba() const {
    return fontRgba_;
}

void ImageState::setText(const UnicodeString& text) {
    overrideText_ = true;
    text_ = text;

    owner_->request_repaint();
}

UnicodeString ImageState::getText() const {
    return text_;
}

void ImageState::setTiled(bool value) {
    overrideTiled_ = true;
    tiled_ = value;

    owner_->request_repaint();
}

bool ImageState::getTiled() const {
    return tiled_;
}

void ImageState::setPartialHue(bool value) {
    overridePartialHue_ = true;
    partialHue_ = value;

    owner_->request_repaint();
}

bool ImageState::getPartialHue() const {
    return partialHue_;
}

CL_Texture ImageState::getTileableTexture() {
    if (!texture_ || !texture_->isReadComplete()) {
        return CL_Texture();
    }

    if (tileableTexture_.is_null()) {
        // create CL_Texture from ui::Texture. can be very expensive, unfortunately.
        // maybe find a better solution (tile and crop textures manually here instead of relying on tex coords?)
        tileableTexture_ = texture_->extractSingleTexture();
        tileableTexture_.set_wrap_mode(cl_wrap_repeat , cl_wrap_repeat );
    }

    return tileableTexture_;
}

boost::python::tuple ImageState::pyGetRgba() const {
    CL_Colorf rgba = getRgba();
    return boost::python::make_tuple(rgba.r, rgba.g, rgba.b, rgba.a);
}

void ImageState::pySetRgba(const boost::python::tuple& rgba) {
    namespace bpy = boost::python;

    float r = bpy::extract<float>(rgba[0]);
    float g = bpy::extract<float>(rgba[1]);
    float b = bpy::extract<float>(rgba[2]);

    if (bpy::len(rgba) > 3) {
        float a = bpy::extract<float>(rgba[3]);
        setRgba(r, g, b, a);
    } else {
        setRgba(r, g, b);
    }
}

boost::python::tuple ImageState::pyGetFontRgba() const {
    CL_Colorf rgba = getFontRgba();
    return boost::python::make_tuple(rgba.r, rgba.g, rgba.b, rgba.a);
}

void ImageState::pySetFontRgba(const boost::python::tuple& rgba) {
    namespace bpy = boost::python;

    float r = bpy::extract<float>(rgba[0]);
    float g = bpy::extract<float>(rgba[1]);
    float b = bpy::extract<float>(rgba[2]);

    if (bpy::len(rgba) > 3) {
        float a = bpy::extract<float>(rgba[3]);
        setFontRgba(r, g, b, a);
    } else {
        setFontRgba(r, g, b);
    }
}





Image::Image(CL_GUIComponent* parent) : GumpComponent(parent),
        autoResize_(false), defaultState_(nullptr), overrideGumpFont_(false),
        vAlign_(MIDDLE), hAlign_(CENTER) {
    func_render().set(this, &Image::render);

    set_type_name("image");

    hasScrollareaParent_ = false;
    CL_GUIComponent* comp = parent;
    while (comp) {
        if (dynamic_cast<components::ScrollArea*>(comp)) {
            hasScrollareaParent_ = true;
            break;
        }
        comp = comp->get_parent_component();
    }

    setCurrentState("normal");
}

void Image::render(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    CL_Rectf geom = get_geometry();
    boost::shared_ptr<ui::Texture> tex = getTexture();
    if (tex) {
        if (!tex->isReadComplete()) {
            // texture not loaded yet
            ui::Manager::getSingleton()->queueComponentRepaint(this);
            return;
        } else if (autoResize_ && (geom.get_width() != tex->getWidth() || geom.get_height() != tex->getHeight())) {
            geom.set_width(tex->getWidth());
            geom.set_height(tex->getHeight());

            ui::Manager::getSingleton()->queueComponentResize(this, geom);
            // repainted automatically after resize
            return;
        }

        // component is ready to be rendered
        if (!getTiled()) {
            if (useRgba()) {
                CL_Draw::texture(gc, tex->getTexture(), CL_Quadf(CL_Rectf(0, 0, get_width(), get_height())), getRgba(), tex->getNormalizedTextureCoords());
            } else {
                renderShader(gc, clipRect);
            }
        } else {
            if (useRgba()) {
                CL_Draw::texture(gc, getTileableTexture(),
                        CL_Quadf(CL_Rectf(0, 0, get_width(), get_height())),
                        getRgba(),
                        CL_Rectf(0.0f, 0.0f, get_width() / tex->getWidth(), get_height() / tex->getHeight()));
            } else {
                renderShader(gc, clipRect);
            }
        }
    }

    // paint text
    UnicodeString txt = getText();
    if (txt.length() > 0) {
        CL_SpanLayout span;
        CL_Font font = overrideGumpFont_ ? cachedFont_ : getGumpMenu()->getFont();
        span.add_text(StringConverter::toUtf8String(txt), font, getFontRgba());
        span.set_align((CL_SpanAlign)getHAlign());

        span.layout(gc, get_geometry().get_width());
        CL_Size spanSize = span.get_size();

        if (!hasScrollareaParent_) {
            gc.push_cliprect(get_geometry());
        }
        // span aligns only horizontally. vertical alignment needs to be done manually
        switch (getVAlign()) {
        case TOP:
            span.set_position(CL_Point(0, 0));
            break;
        case MIDDLE:
            span.set_position(CL_Point(0, (get_height() - spanSize.height) / 2));
            break;
        case BOTTOM:
            span.set_position(CL_Point(0, (get_height() - spanSize.height)));
            break;
        }
        span.draw_layout(gc);
        span.set_component_geometry();

        if (!hasScrollareaParent_) {
            gc.pop_cliprect();
        }
    }
}

void Image::setAutoResize(bool value) {
    autoResize_ = value;
}

void Image::renderShader(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    boost::shared_ptr<CL_ProgramObject> shader = ui::Manager::getShaderManager()->getGumpShader();
    gc.set_program_object(*shader, cl_program_matrix_modelview_projection);

    CL_Texture huesTexture = data::Manager::getHuesLoader()->getHuesTexture();
    gc.set_texture(0, huesTexture);
    // set texture unit 1 active to avoid overriding the hue texture with newly loaded object textures
    gc.set_texture(1, huesTexture);

    shader->set_uniform1i("HueTexture", 0);
    shader->set_uniform1i("ObjectTexture", 1);

    CL_Rectf textureRect;

    if (getTiled()) {
        CL_Texture tex = getTileableTexture();
        gc.set_texture(1, tex);
        textureRect = CL_Rectf(0.0f, 0.0f, get_width() / tex.get_width(), get_height() / tex.get_height());
    } else {
        boost::shared_ptr<ui::Texture> tex = getTexture();
        gc.set_texture(1, tex->getTexture());
        textureRect = tex->getNormalizedTextureCoords();
    }

    CL_Vec2f tex1_coords[6] = {
        CL_Vec2f(textureRect.left, textureRect.top),
        CL_Vec2f(textureRect.right, textureRect.top),
        CL_Vec2f(textureRect.left, textureRect.bottom),
        CL_Vec2f(textureRect.right, textureRect.top),
        CL_Vec2f(textureRect.left, textureRect.bottom),
        CL_Vec2f(textureRect.right, textureRect.bottom)
    };

    CL_Vec2f vertexCoords[6];

    CL_Rectf rect(0, 0, get_width(), get_height());

    vertexCoords[0] = CL_Vec2f(rect.left, rect.top);
    vertexCoords[1] = CL_Vec2f(rect.right, rect.top);
    vertexCoords[2] = CL_Vec2f(rect.left, rect.bottom);
    vertexCoords[3] = CL_Vec2f(rect.right, rect.top);
    vertexCoords[4] = CL_Vec2f(rect.left, rect.bottom);
    vertexCoords[5] = CL_Vec2f(rect.right, rect.bottom);

    CL_PrimitivesArray primarray(gc);
    primarray.set_attributes(0, vertexCoords);
    primarray.set_attributes(1, tex1_coords);

    CL_Vec3f hueVec = getHueVector();
    primarray.set_attribute(2, hueVec);

    gc.draw_primitives(cl_triangles, 6, primarray);

    gc.reset_textures();
    gc.reset_program_object();
}

bool Image::has_pixel(const CL_Point& p) const {
    boost::shared_ptr<ui::Texture> tex = getTexture();
    if (!tex || !tex->isReadComplete()) {
        return false;
    }

    int px = p.x;
    int py = p.y;

    if (getTiled()) {
        px %= (int)tex->getWidth();
        py %= (int)tex->getHeight();
    } else {
        float stretchHori = (float)get_width() / tex->getWidth();
        float stretchVert = (float)get_height() / tex->getHeight();
        px /= stretchHori;
        py /= stretchVert;
    }

    return tex->hasPixel(px, py);
}

void Image::setCurrentState(const UnicodeString& name) {
    currentStateName_ = name;
    currentState_ = getState(name);

    if (!defaultState_) {
        defaultState_ = currentState_;
    }

    request_repaint();
}

UnicodeString Image::getCurrentStateName() const {
    return currentStateName_;
}

ImageState* Image::getState(const UnicodeString& name) {
    ImageState* ret = &states_[name];
    ret->setOwner(this);
    return ret;
}

boost::shared_ptr<ui::Texture> Image::getTexture() const {
    if (defaultState_ != currentState_ && currentState_->overrideTexture_) {
        return currentState_->getTexture();
    } else {
        return defaultState_->getTexture();
    }
}

void Image::setTexture(boost::shared_ptr<ui::Texture> tex) {
    defaultState_->setTexture(tex);

    request_repaint();
}

CL_Texture Image::getTileableTexture() {
    if (defaultState_ != currentState_ && currentState_->overrideTexture_) {
        return currentState_->getTileableTexture();
    } else {
        return defaultState_->getTileableTexture();
    }
}

unsigned int Image::getHue() const {
    if (defaultState_ != currentState_ && currentState_->overrideHue_) {
        return currentState_->getHue();
    } else {
        return defaultState_->getHue();
    }
}

void Image::setHue(unsigned int hue) {
    defaultState_->setHue(hue);

    request_repaint();
}

bool Image::getTiled() const {
    if (defaultState_ != currentState_ && currentState_->overrideTiled_) {
        return currentState_->getTiled();
    } else {
        return defaultState_->getTiled();
    }
}

void Image::setTiled(bool value) {
    defaultState_->setTiled(value);

    request_repaint();
}

CL_Colorf Image::getRgba() const {
    if (defaultState_ != currentState_ && currentState_->overrideRgba_) {
        return currentState_->getRgba();
    } else {
        return defaultState_->getRgba();
    }
}

void Image::setRgba(const CL_Colorf& rgba) {
    defaultState_->setRgba(rgba);

    request_repaint();
}

void Image::setRgba(float r, float g, float b, float a) {
    setRgba(CL_Colorf(r, g, b, a));

    request_repaint();
}

void Image::setRgba(float r, float g, float b) {
    setRgba(CL_Colorf(r, g, b));

    request_repaint();
}

bool Image::useRgba() const {
    if (currentState_->overrideRgba_) {
        return true;
    } else if (currentState_->overrideHue_) {
        return false;
    } else if (defaultState_->overrideRgba_) {
        return true;
    } else {
        return false;
    }
}

void Image::setPartialHue(bool value) {
    defaultState_->setPartialHue(value);

    request_repaint();
}

bool Image::getPartialHue() const {
    if (defaultState_ != currentState_ && currentState_->overridePartialHue_) {
        return currentState_->getPartialHue();
    } else {
        return defaultState_->getPartialHue();
    }
}

CL_Vec3f Image::getHueVector() const {
    return CL_Vec3f((getPartialHue() ? 1 : 0), getHue(), getAlpha());
}

void Image::setAlpha(float alpha) {
    defaultState_->setAlpha(alpha);

    request_repaint();
}

float Image::getAlpha() const {
    if (defaultState_ != currentState_ && currentState_->overrideAlpha_) {
        return currentState_->getAlpha();
    } else {
        return defaultState_->getAlpha();
    }
}

CL_Colorf Image::getFontRgba() const {
    if (defaultState_ != currentState_ && currentState_->overrideFontRgba_) {
        return currentState_->getFontRgba();
    } else {
        return defaultState_->getFontRgba();
    }
}

void Image::setFontRgba(const CL_Colorf& rgba) {
    defaultState_->setFontRgba(rgba);

    request_repaint();
}

void Image::setFontRgba(float r, float g, float b, float a) {
    setFontRgba(CL_Colorf(r, g, b, a));
}

void Image::setFontRgba(float r, float g, float b) {
    setFontRgba(CL_Colorf(r, g, b));
}

UnicodeString Image::getText() const {
    if (defaultState_ != currentState_ && currentState_->overrideText_) {
        return currentState_->getText();
    } else {
        return defaultState_->getText();
    }
}

void Image::setText(const UnicodeString& text) {
    defaultState_->setText(text);

    request_repaint();
}

void Image::setFontB(const UnicodeString& name, unsigned int height, bool border) {
    fontDesc_.set_typeface_name(StringConverter::toUtf8String(name));
    fontDesc_.set_height(height);
    fontDesc_.set_weight(border ? 700 : 400); // weight values taken from clanlib
    fontDesc_.set_subpixel(false);

    cachedFont_ = ui::Manager::getSingleton()->getFont(fontDesc_);
    overrideGumpFont_ = true;
}

void Image::setFont(const UnicodeString& name, unsigned int height) {
    setFontB(name, height, false);
}

void Image::setVAlign(VAlign align) {
    vAlign_ = align;

    request_repaint();
}

VAlign Image::getVAlign() const {
    return vAlign_;
}

void Image::setHAlign(HAlign align) {
    hAlign_ = align;

    request_repaint();
}

HAlign Image::getHAlign() const {
    return hAlign_;
}

boost::python::tuple Image::pyGetRgba() const {
    CL_Colorf rgba = getRgba();
    return boost::python::make_tuple(rgba.r, rgba.g, rgba.b, rgba.a);
}

void Image::pySetRgba(const boost::python::tuple& rgba) {
    namespace bpy = boost::python;

    float r = bpy::extract<float>(rgba[0]);
    float g = bpy::extract<float>(rgba[1]);
    float b = bpy::extract<float>(rgba[2]);

    if (bpy::len(rgba) > 3) {
        float a = bpy::extract<float>(rgba[3]);
        setRgba(r, g, b, a);
    } else {
        setRgba(r, g, b);
    }
}

boost::python::tuple Image::pyGetFontRgba() const {
    CL_Colorf rgba = getFontRgba();
    return boost::python::make_tuple(rgba.r, rgba.g, rgba.b, rgba.a);
}

void Image::pySetFontRgba(const boost::python::tuple& rgba) {
    namespace bpy = boost::python;

    float r = bpy::extract<float>(rgba[0]);
    float g = bpy::extract<float>(rgba[1]);
    float b = bpy::extract<float>(rgba[2]);

    if (bpy::len(rgba) > 3) {
        float a = bpy::extract<float>(rgba[3]);
        setFontRgba(r, g, b, a);
    } else {
        setFontRgba(r, g, b);
    }
}

}
}
}

