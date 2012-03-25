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



#include "startlocationprovider.hpp"

#include <stdlib.h>

#include <misc/random.hpp>

namespace fluo {
namespace ui {
namespace particles {

CL_Vec3f StartLocationProviderEmitter::get(const CL_Vec3f& emitterLocation) const {
    return emitterLocation;
}

void StartLocationProviderEmitter::setNormalizedAge(float age) {
    // do nothing
}


void StartLocationProviderWithSize::setSize(float widthStart, float widthEnd, float heightStart, float heightEnd) {
    widthHalf_ = InterpolatedValue<float>(widthStart / 2, widthEnd / 2);
    heightHalf_ =  InterpolatedValue<float>(heightStart / 2, heightEnd / 2);
}

void StartLocationProviderWithSize::setSizeT1(float width, float height) {
    widthHalf_.setT1(width / 2);
    heightHalf_.setT1(height / 2);
}

void StartLocationProviderWithSize::setNormalizedAge(float age) {
    widthHalf_.setNormalizedAge(age);
    heightHalf_.setNormalizedAge(age);
}


CL_Vec3f StartLocationProviderBox::get(const CL_Vec3f& emitterLocation) const {
    CL_Vec3f ret(emitterLocation);

    ret.x += Random::randomMinMax(-widthHalf_.get(), widthHalf_.get());
    ret.y += Random::randomMinMax(-heightHalf_.get(), heightHalf_.get());

    return ret;
}


CL_Vec3f StartLocationProviderBoxOutline::get(const CL_Vec3f& emitterLocation) const {
    CL_Vec3f ret(emitterLocation);

    if (Random::randomBool()) {
        ret.x += Random::randomBool() ? -heightHalf_.get() : heightHalf_.get();
        ret.y += Random::randomMinMax(-widthHalf_.get(), widthHalf_.get());
    } else {
        ret.x += Random::randomMinMax(-heightHalf_.get(), heightHalf_.get());
        ret.y += Random::randomBool() ? -widthHalf_.get() : widthHalf_.get();
    }

    return ret;
}


CL_Vec3f StartLocationProviderOval::get(const CL_Vec3f& emitterLocation) const {
    CL_Vec3f ret(emitterLocation);

    float theta = Random::random01() * 6.283185;
    float length = Random::random01();

    ret.x += cos(theta) * widthHalf_.get() * length;
    ret.y += sin(theta) * heightHalf_.get() * length;

    return ret;
}


CL_Vec3f StartLocationProviderOvalOutline::get(const CL_Vec3f& emitterLocation) const {
    CL_Vec3f ret(emitterLocation);

    float theta = Random::random01() * 6.283185;

    ret.x += cos(theta) * widthHalf_.get();
    ret.y += sin(theta) * heightHalf_.get();

    return ret;
}

}
}
}
