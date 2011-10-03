#ifndef FLUO_WORLD_LIGHTMANAGER_HPP
#define FLUO_WORLD_LIGHTMANAGER_HPP

#include <ClanLib/Core/Math/vec3.h>

namespace fluo {
namespace world {

class LightManager {
public:
    LightManager();

    CL_Vec3f getAmbientIntensity() const;
    void setAmbientIntensity(const CL_Vec3f& value);

    CL_Vec3f getGlobalIntensity() const;
    void setGlobalIntensity(const CL_Vec3f& value);

    CL_Vec3f getGlobalDirection() const;
    void setGlobalDirection(const CL_Vec3f& direction);

    void setGlobalAngle(float angle);
    float getGlobalAngle() const;

private:
    CL_Vec3f ambientIntensity_;
    CL_Vec3f globalIntensity_;
    float globalAngle_;
    CL_Vec3f globalDirection_;
};

}
}

#endif
