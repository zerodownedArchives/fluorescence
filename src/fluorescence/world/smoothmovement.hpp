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


#ifndef FLUO_WORLD_SMOOTHMOVEMENT_HPP
#define FLUO_WORLD_SMOOTHMOVEMENT_HPP

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <ClanLib/Core/Math/vec3.h>

#include <world/serverobject.hpp>

namespace fluo {
namespace world {

class SmoothMovement {
public:
    typedef boost::function<void ()> StartCallback;
    typedef boost::function<void ()> FinishedCallback;

    SmoothMovement(boost::shared_ptr<world::ServerObject> obj, CL_Vec3f targetLoc_, unsigned int durationMillis);
    SmoothMovement(boost::shared_ptr<world::ServerObject> obj, unsigned int direction, CL_Vec3f targetLoc);

    bool wasStarted() const;
    void start();

    // returns >0 if at the end of this movement there are still some millis lef
    unsigned int update(unsigned int elapsedMillis);

    bool isFinished() const;
    void finish(bool interrupted);

    void setStartCallback(StartCallback cb);
    void setFinishedCallback(FinishedCallback cb);

    CL_Vec3f getTargetLoc() const;

private:
    boost::shared_ptr<world::ServerObject> movingObject_;
    CL_Vec3f targetLoc_;
    CL_Vec3f diff_;
    unsigned int duration_;

    bool wasStarted_;
    CL_Vec3f startLoc_;
    unsigned int totalElapsedMillis_;

    StartCallback startCallback_;
    FinishedCallback finishedCallback_;

    bool isDirectionChange_;
    unsigned int newDirection_;
};

}
}

#endif
