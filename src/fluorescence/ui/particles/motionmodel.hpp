#ifndef FLUO_UI_PARTICLES_MOTIONMODEL_HPP
#define FLUO_UI_PARTICLES_MOTIONMODEL_HPP

#include <ClanLib/Core/Math/vec3.h>

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
    CL_Vec3f startVelocityMinT0_;
    CL_Vec3f startVelocityMinT1_;
    CL_Vec3f startVelocityMaxT0_;
    CL_Vec3f startVelocityMaxT1_;
    CL_Vec3f endVelocityMinT0_;
    CL_Vec3f endVelocityMinT1_;
    CL_Vec3f endVelocityMaxT0_;
    CL_Vec3f endVelocityMaxT1_;

    CL_Vec3f startVelocityMinTx_;
    CL_Vec3f startVelocityMaxTx_;
    CL_Vec3f endVelocityMinTx_;
    CL_Vec3f endVelocityMaxTx_;
};


class MotionModelAwayFromEmitter : public MotionModel {
public:
    MotionModelAwayFromEmitter();

    void setAccelerationT0(float startMin, float startMax, float endMin, float endMax);
    void setAccelerationT1(float startMin, float startMax, float endMin, float endMax);

    virtual void setNormalizedAge(float age);
    virtual void get(const CL_Vec3f& emitterPosition, const CL_Vec3f& particlePosition, CL_Vec3f& param1, CL_Vec3f& param2) const;

private:
    float startAccelerationMinT0_;
    float startAccelerationMinT1_;
    float startAccelerationMaxT0_;
    float startAccelerationMaxT1_;
    float endAccelerationMinT0_;
    float endAccelerationMinT1_;
    float endAccelerationMaxT0_;
    float endAccelerationMaxT1_;

    float startAccelerationMinTx_;
    float startAccelerationMaxTx_;
    float endAccelerationMinTx_;
    float endAccelerationMaxTx_;
};


}
}
}

#endif
