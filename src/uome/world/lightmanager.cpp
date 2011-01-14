
#include "lightmanager.hpp"

namespace uome {
namespace world {

LightManager::LightManager() :
        ambientIntensity_(1, 1, 1) {
}

CL_Vec3f LightManager::getAmbientIntensity() const {
    return ambientIntensity_;
}

void LightManager::setAmbientIntensity(CL_Vec3f value) {
    ambientIntensity_ = value;
}

}
}
