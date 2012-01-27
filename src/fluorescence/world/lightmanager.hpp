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
