
#include "lightmanager.hpp"

#include <stdlib.h>

namespace uome {
namespace world {

LightManager::LightManager() :
        ambientIntensity_(0.3, 0.3, 0.3), globalIntensity_(0.2, 0.2, 0.2) {
    setGlobalAngle(150);
}

CL_Vec3f LightManager::getAmbientIntensity() const {
    return ambientIntensity_;
}

void LightManager::setAmbientIntensity(const CL_Vec3f& value) {
    ambientIntensity_ = value;
}

CL_Vec3f LightManager::getGlobalIntensity() const {
    return globalIntensity_;
}

void LightManager::setGlobalIntensity(const CL_Vec3f& value) {
    globalIntensity_ = value;
}

CL_Vec3f LightManager::getGlobalDirection() const {
    return globalDirection_;
}

void LightManager::setGlobalDirection(const CL_Vec3f& direction) {
    globalDirection_ = direction;
}

void LightManager::setGlobalAngle(float angle) {
    globalAngle_ = angle;
    float angleRad = angle * M_PI / 360.0f;
    globalDirection_ = CL_Vec3f(-cos(angleRad), 0, sin(angleRad)).normalize();
}

float LightManager::getGlobalAngle() const {
    return globalAngle_;
}

}
}
