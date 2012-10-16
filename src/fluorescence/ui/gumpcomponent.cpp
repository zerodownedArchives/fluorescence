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



#include "gumpcomponent.hpp"

namespace fluo {
namespace ui {

GumpComponent::GumpComponent(CL_GUIComponent* parent) :
    CL_GUIComponent(parent),
    useRgba_(false), hue_(0), usePartialHue_(false), alpha_(1.0), rgba_(CL_Colorf::white) {

    set_double_click_enabled(false);
}

int GumpComponent::getX() const {
    return get_geometry().left;
}

void GumpComponent::setX(int x) {
    CL_Rectf geom = get_geometry();
    int diff = x - geom.left;
    geom.translate(diff, 0);
    set_geometry(geom);
}

int GumpComponent::getY() const {
    return get_geometry().top;
}

void GumpComponent::setY(int y) {
    CL_Rectf geom = get_geometry();
    int diff = y - geom.top;
    geom.translate(0, diff);
    set_geometry(geom);
}

int GumpComponent::getWidth() const {
    return get_geometry().get_width();
}

void GumpComponent::setWidth(int w) {
    CL_Rectf geom = get_geometry();
    geom.set_width(w);
    set_geometry(geom);
}

int GumpComponent::getHeight() const {
    return get_geometry().get_height();
}

void GumpComponent::setHeight(int h) {
    CL_Rectf geom = get_geometry();
    geom.set_height(h);
    set_geometry(geom);
}

CL_Rectf GumpComponent::getGeometry() const {
    return get_geometry();
}

void GumpComponent::setGeometry(int x, int y, int w, int h) {
    CL_Rectf geom(x, y, CL_Sizef(w, h));
    set_geometry(geom);
}

bool GumpComponent::useRgba() const {
    return useRgba_;
}

void GumpComponent::setUseRgba(bool value) {
    useRgba_ = value;
}

bool GumpComponent::usePartialHue() const {
    return usePartialHue_;
}

void GumpComponent::setUsePartialHue(bool value) {
    usePartialHue_ = value;
}

CL_Colorf GumpComponent::getRgba() const {
    return rgba_;
}

void GumpComponent::setRgba(const CL_Colorf& rgba) {
    rgba_ = rgba;

    setUseRgba(true);
}

void GumpComponent::setRgba(float r, float g, float b, float a) {
    rgba_.r = r;
    rgba_.g = g;
    rgba_.b = b;
    rgba_.a = a;

    setUseRgba(true);
}

void GumpComponent::setRgba(float r, float g, float b) {
    rgba_.r = r;
    rgba_.g = g;
    rgba_.b = b;

    setUseRgba(true);
}

unsigned int GumpComponent::getHue() const {
    return hue_;
}

void GumpComponent::setHue(unsigned int hue) {
    hue_ = hue;
    setUseRgba(false);
}

float GumpComponent::getAlpha() const {
    return alpha_;
}

void GumpComponent::setAlpha(float alpha) {
    alpha_ = alpha;
    rgba_.a = alpha;
}

CL_Vec3f GumpComponent::getHueVector() const {
    return CL_Vec3f((usePartialHue_ ? 1 : 0), hue_, alpha_);
}


void GumpComponent::setAutoResize(bool value) {
}

}
}
