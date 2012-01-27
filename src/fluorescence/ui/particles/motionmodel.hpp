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


#ifndef FLUO_UI_PARTICLES_MOTIONMODEL_HPP
#define FLUO_UI_PARTICLES_MOTIONMODEL_HPP

#include <ClanLib/Core/Math/vec3.h>

#include <misc/interpolation.hpp>

namespace fluo {
namespace ui {
namespace particles {

class MotionModel {
public:
    virtual void setNormalizedAge(float age) = 0;
    virtual void get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const = 0;
};


class MotionModelStatic : public MotionModel {
public:
    virtual void setNormalizedAge(float age);
    virtual void get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const;
};


class MotionModelStartEndVelocity : public MotionModel {
public:
    MotionModelStartEndVelocity();

    void setVelocitiesT0(const CL_Vec3f& startMin, const CL_Vec3f& startMax, const CL_Vec3f& endMin, const CL_Vec3f& endMax);
    void setVelocitiesT1(const CL_Vec3f& startMin, const CL_Vec3f& startMax, const CL_Vec3f& endMin, const CL_Vec3f& endMax);

    void setVelocityAndAccelerationT0(const CL_Vec3f& startMin, const CL_Vec3f& startMax, float accelMin, float accelMax);
    void setVelocityAndAccelerationT1(const CL_Vec3f& startMin, const CL_Vec3f& startMax, float accelMin, float accelMax);

    virtual void setNormalizedAge(float age);
    virtual void get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const;

private:
    InterpolatedValue<CL_Vec3f> startVelocityMin_;
    InterpolatedValue<CL_Vec3f> startVelocityMax_;
    InterpolatedValue<CL_Vec3f> endVelocityMin_;
    InterpolatedValue<CL_Vec3f> endVelocityMax_;
};


class MotionModelAwayFromEmitter : public MotionModel {
public:
    MotionModelAwayFromEmitter();

    void setAccelerationT0(float startMin, float startMax, float endMin, float endMax);
    void setAccelerationT1(float startMin, float startMax, float endMin, float endMax);

    virtual void setNormalizedAge(float age);
    virtual void get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const;

private:
    InterpolatedValue<float> startAccelerationMin_;
    InterpolatedValue<float> startAccelerationMax_;
    InterpolatedValue<float> endAccelerationMin_;
    InterpolatedValue<float> endAccelerationMax_;
};


}
}
}

#endif
