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



#include "random.hpp"

#include <stdlib.h>

namespace fluo {

bool Random::randomBool() {
    return random01() < 0.5;
}

float Random::random01() {
    return ((float)rand() / RAND_MAX);
}

float Random::randomMinMax(float min, float max) {
    return min + random01() * (max - min);
}

unsigned int Random::randomMinMax(unsigned int min, unsigned int max) {
    return min + random01() * (max - min);
}

CL_Vec3f Random::randomMinMax(const CL_Vec3f& min, const CL_Vec3f& max) {
    CL_Vec3f ret;
    ret.x = randomMinMax(min.x, max.x);
    ret.y = randomMinMax(min.y, max.y);
    ret.z = randomMinMax(min.z, max.z);
    return ret;
}

CL_Vec4f Random::randomMinMax(const CL_Vec4f& min, const CL_Vec4f& max) {
    CL_Vec4f ret;
    ret.x = randomMinMax(min.x, max.x);
    ret.y = randomMinMax(min.y, max.y);
    ret.z = randomMinMax(min.z, max.z);
    ret.w = randomMinMax(min.w, max.w);
    return ret;
}

}
