#ifndef UOME_WORLD_LIGHTMANAGER_HPP
#define UOME_WORLD_LIGHTMANAGER_HPP

#include <ClanLib/Core/Math/vec3.h>

namespace uome {
namespace world {

class LightManager {
public:
    LightManager();

    CL_Vec3f getAmbientIntensity() const;
    void setAmbientIntensity(const CL_Vec3f& value);

    CL_Vec3f getGlobalIntensity() const;
    void setGlobalIntensity(const CL_Vec3f& value);

    CL_Vec3f getGlobalDirection() const;
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
