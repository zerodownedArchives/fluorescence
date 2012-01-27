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


#ifndef FLUO_UI_PARTICLES_STARTPOSITIONPROVIDER_HPP
#define FLUO_UI_PARTICLES_STARTPOSITIONPROVIDER_HPP

#include <ClanLib/Core/Math/vec3.h>

#include <misc/interpolation.hpp>

namespace fluo {
namespace ui {
namespace particles {

class StartPositionProvider {
public:
    virtual CL_Vec3f get(const CL_Vec3f& emitterPosition) const = 0;
    virtual void setNormalizedAge(float age) = 0;
};


class StartPositionProviderEmitter {
public:
    virtual CL_Vec3f get(const CL_Vec3f& emitterPosition) const;
    virtual void setNormalizedAge(float age);
};


class StartPositionProviderWithSize : public StartPositionProvider {
public:
    void setSize(float widthStart, float widthEnd, float heightStart, float heightEnd);
    void setSizeT1(float width, float height);
    virtual void setNormalizedAge(float age);

protected:
    InterpolatedValue<float> widthHalf_;
    InterpolatedValue<float> heightHalf_;
};

class StartPositionProviderBox : public StartPositionProviderWithSize {
public:
    virtual CL_Vec3f get(const CL_Vec3f& emitterPosition) const;
};

class StartPositionProviderBoxOutline : public StartPositionProviderWithSize {
public:
    virtual CL_Vec3f get(const CL_Vec3f& emitterPosition) const;
};


class StartPositionProviderOval : public StartPositionProviderWithSize {
public:
    virtual CL_Vec3f get(const CL_Vec3f& emitterPosition) const;
};

class StartPositionProviderOvalOutline : public StartPositionProviderWithSize {
public:
    virtual CL_Vec3f get(const CL_Vec3f& emitterPosition) const;
};

}
}
}


#endif
