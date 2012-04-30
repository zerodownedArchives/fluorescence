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


#ifndef FLUO_RANDOM_HPP
#define FLUO_RANDOM_HPP

#include <ClanLib/Core/Math/vec3.h>
#include <ClanLib/Core/Math/vec4.h>
#include <ClanLib/Display/2D/color.h>

namespace fluo {

class Random {
public:
    static bool randomBool();
    static float random01();
    static float randomMinMax(float min, float max);
    static unsigned int randomMinMax(unsigned int min, unsigned int max);
    static CL_Vec3f randomMinMax(const CL_Vec3f& min, const CL_Vec3f& max);
    static CL_Vec4f randomMinMax(const CL_Vec4f& min, const CL_Vec4f& max);
    static CL_Colorf randomMinMax(const CL_Colorf& min, const CL_Colorf& max);
};

template<typename T>
class RandomizedValue {
public:
    RandomizedValue() {
    }

    RandomizedValue(const T& t0, const T& t1) : valueT0_(t0),valueT1_(t1) {
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

    T get() const {
        return Random::randomMinMax(valueT0_, valueT1_);
    }
    
private:
    T valueT0_;
    T valueT1_;
};

}

#endif
