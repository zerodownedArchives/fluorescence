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



#include "interpolation.hpp"

namespace fluo {

float Interpolation::linear(float a, float b, float factor) {
    return a + (b-a) * factor;
}

CL_Vec2f Interpolation::linear(const CL_Vec2f& a, const CL_Vec2f& b, float factor) {
    CL_Vec2f ret;
    ret.x = linear(a.x, b.x, factor);
    ret.y = linear(a.y, b.y, factor);
    return ret;
}

CL_Vec3f Interpolation::linear(const CL_Vec3f& a, const CL_Vec3f& b, float factor) {
    CL_Vec3f ret;
    ret.x = linear(a.x, b.x, factor);
    ret.y = linear(a.y, b.y, factor);
    ret.z = linear(a.z, b.z, factor);
    return ret;
}

CL_Vec4f Interpolation::linear(const CL_Vec4f& a, const CL_Vec4f& b, float factor) {
    CL_Vec4f ret;
    ret.x = linear(a.x, b.x, factor);
    ret.y = linear(a.y, b.y, factor);
    ret.z = linear(a.z, b.z, factor);
    ret.w = linear(a.w, b.w, factor);
    return ret;
}

}
