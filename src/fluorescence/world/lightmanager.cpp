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



#include "lightmanager.hpp"

#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <ui/manager.hpp>
#include <ui/render/renderqueue.hpp>

namespace fluo {
namespace world {

LightManager::LightManager() :
        ambientIntensity_(0.8, 0.8, 0.8), globalIntensity_(0.4, 0.4, 0.4) {
    setGlobalAngle(45);
}

CL_Vec3f LightManager::getAmbientIntensity() const {
    return ambientIntensity_;
}

void LightManager::setAmbientIntensity(const CL_Vec3f& value) {
    ambientIntensity_ = value;

    ui::Manager::getWorldRenderQueue()->forceRepaint();
}

CL_Vec3f LightManager::getGlobalIntensity() const {
    return globalIntensity_;
}

void LightManager::setGlobalIntensity(const CL_Vec3f& value) {
    globalIntensity_ = value;

    ui::Manager::getWorldRenderQueue()->forceRepaint();
}

CL_Vec3f LightManager::getGlobalDirection() const {
    return globalDirection_;
}

void LightManager::setGlobalDirection(const CL_Vec3f& direction) {
    globalDirection_ = direction;

    ui::Manager::getWorldRenderQueue()->forceRepaint();
}

void LightManager::setGlobalAngle(float angle) {
    globalAngle_ = angle;
    float angleRad = angle * M_PI / 360.0f;
    globalDirection_ = CL_Vec3f(-cos(angleRad), 0, sin(angleRad)).normalize();

    ui::Manager::getWorldRenderQueue()->forceRepaint();
}

float LightManager::getGlobalAngle() const {
    return globalAngle_;
}

}
}
