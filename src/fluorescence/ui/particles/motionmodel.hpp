#ifndef FLUO_UI_PARTICLES_MOTIONMODEL_HPP
#define FLUO_UI_PARTICLES_MOTIONMODEL_HPP

#include <ClanLib/Core/Math/vec3.h>

namespace fluo {
namespace ui {
namespace particles {

class MotionModel {
public:
    enum {
        TYPE_START_END_VELOCITY = 0,
        TYPE_AWAY_FROM_EMITTER = 1,
    };


    MotionModel(unsigned int id);

    unsigned int getId() const;

    virtual void setNormalizedAge(float age) = 0;
    virtual void get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const = 0;

private:
    unsigned int id_;
};


class MotionModelStartEndVelocity : public MotionModel {
public:
    MotionModelStartEndVelocity();

    void setStartVelocities(const CL_Vec3f& startMin, const CL_Vec3f& startMax, const CL_Vec3f& endMin, const CL_Vec3f& endMax);
    void setEndVelocities(const CL_Vec3f& startMin, const CL_Vec3f& startMax, const CL_Vec3f& endMin, const CL_Vec3f& endMax);

    virtual void setNormalizedAge(float age);
    virtual void get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const;

private:
    CL_Vec3f startVelocityMinStart_;
    CL_Vec3f startVelocityMinEnd_;
    CL_Vec3f startVelocityMaxStart_;
    CL_Vec3f startVelocityMaxEnd_;
    CL_Vec3f endVelocityMinStart_;
    CL_Vec3f endVelocityMinEnd_;
    CL_Vec3f endVelocityMaxStart_;
    CL_Vec3f endVelocityMaxEnd_;

    CL_Vec3f startVelocityMinCur_;
    CL_Vec3f startVelocityMaxCur_;
    CL_Vec3f endVelocityMinCur_;
    CL_Vec3f endVelocityMaxCur_;
};


class MotionModelAwayFromEmitter : public MotionModel {
public:
    MotionModelAwayFromEmitter();

    void setStartAcceleration(float startMin, float startMax, float endMin, float endMax);
    void setEndAcceleration(float startMin, float startMax, float endMin, float endMax);

    virtual void setNormalizedAge(float age);
    virtual void get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const;

private:
    float startAccelerationMinStart_;
    float startAccelerationMinEnd_;
    float startAccelerationMaxStart_;
    float startAccelerationMaxEnd_;
    float endAccelerationMinStart_;
    float endAccelerationMinEnd_;
    float endAccelerationMaxStart_;
    float endAccelerationMaxEnd_;

    float startAccelerationMinCur_;
    float startAccelerationMaxCur_;
    float endAccelerationMinCur_;
    float endAccelerationMaxCur_;
};

}
}
}

#endif
