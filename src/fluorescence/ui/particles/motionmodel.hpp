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
    };


    MotionModel(unsigned int id);

    unsigned int getId() const;

    virtual void setNormalizedAge(float age) = 0;
    virtual CL_Vec3f getParam1(const CL_Vec3f& emitterPosition) const = 0;
    virtual CL_Vec3f getParam2(const CL_Vec3f& emitterPosition) const = 0;

protected:
    static CL_Vec3f interpolate(const CL_Vec3f& a, const CL_Vec3f& b, float factor);

private:
    unsigned int id_;
};


class MotionModelStartEndVelocity : public MotionModel {
public:
    MotionModelStartEndVelocity();

    void setStartVelocities(const CL_Vec3f& startMin, const CL_Vec3f& startMax, const CL_Vec3f& endMin, const CL_Vec3f& endMax);
    void setEndVelocities(const CL_Vec3f& startMin, const CL_Vec3f& startMax, const CL_Vec3f& endMin, const CL_Vec3f& endMax);

    virtual void setNormalizedAge(float age);
    virtual CL_Vec3f getParam1(const CL_Vec3f& emitterPosition) const;
    virtual CL_Vec3f getParam2(const CL_Vec3f& emitterPosition) const;

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

}
}
}

#endif
