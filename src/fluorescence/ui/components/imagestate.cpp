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

#include "imagestate.hpp"

#include <boost/python/extract.hpp>

#include <ui/texture.hpp>
#include <ui/gumpcomponent.hpp>

namespace fluo {
namespace ui {
namespace components {

ImageState::ImageState() :
        owner_(nullptr),
        overrideTexture_(false),
        overrideHue_(false), hue_(0),
        overrideRgba_(false), rgba_(CL_Colorf::white),
        overrideAlpha_(false), alpha_(1),
        overrideFontRgba_(false), fontRgba_(CL_Colorf::black),
        overrideText_(false),
        overrideTiled_(false), tiled_(false),
        overridePartialHue_(false), partialHue_(false) {
}

void ImageState::setOwner(GumpComponent* owner) {
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

}
}
}

