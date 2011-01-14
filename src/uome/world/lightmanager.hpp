#ifndef UOME_WORLD_LIGHTMANAGER_HPP
#define UOME_WORLD_LIGHTMANAGER_HPP

#include <ClanLib/Core/Math/vec3.h>

namespace uome {
namespace world {

class LightManager {
public:
    LightManager();

    CL_Vec3f getAmbientIntensity() const;
    void setAmbientIntensity(CL_Vec3f value);

private:
    CL_Vec3f ambientIntensity_;
};

}
}

#endif
