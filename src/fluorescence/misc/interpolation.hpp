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


#ifndef FLUO_INTERPOLATION_HPP
#define FLUO_INTERPOLATION_HPP

#include <ClanLib/Core/Math/vec3.h>
#include <ClanLib/Core/Math/vec4.h>

namespace fluo {

class Interpolation {
public:
    static float linear(float a, float b, float factor);
    static CL_Vec3f linear(const CL_Vec3f& a, const CL_Vec3f& b, float factor);
    static CL_Vec4f linear(const CL_Vec4f& a, const CL_Vec4f& b, float factor);
};

template<typename T>
class InterpolatedValue {
public:
    InterpolatedValue() {
    }

    InterpolatedValue(const T& t0, const T& t1) : valueT0_(t0),valueT1_(t1), valueCur_(t0) {
    }

    void set(const T& t0, const T& t1) {
        valueT0_ = t0;
        valueT1_ = t1;
    }

    void setT0(const T& t0) {
        valueT0_ = t0;
    }

    void setT1(const T& t1) {
        valueT1_ = t1;
    }

    const T& get() const {
        return valueCur_;
    }

    void setNormalizedAge(float age) {
        valueCur_ = Interpolation::linear(valueT0_, valueT1_, age);
    }

    operator T() const {
        return valueCur_;
    }

private:
    T valueT0_;
    T valueT1_;
    T valueCur_;
};

}

#endif
