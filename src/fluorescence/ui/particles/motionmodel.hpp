#ifndef FLUO_UI_PARTICLES_MOTIONMODEL_HPP
#define FLUO_UI_PARTICLES_MOTIONMODEL_HPP

#include <ClanLib/Core/Math/vec3.h>

#include <misc/interpolation.hpp>

namespace fluo {
namespace ui {
namespace particles {

class MotionModel {
public:
    enum {
        TYPE_STATIC = 0,
        TYPE_START_END_VELOCITY = 1,
        TYPE_AWAY_FROM_EMITTER = 2,
    };


    MotionModel(unsigned int id);

    unsigned int getId() const;

    virtual void setNormalizedAge(float age) = 0;
    virtual void get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const = 0;

private:
    unsigned int id_;
};


class MotionModelStatic : public MotionModel {
public:
    MotionModelStatic();

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
