#ifndef FLUO_WORLD_SMOOTHMOVEMENT_HPP
#define FLUO_WORLD_SMOOTHMOVEMENT_HPP

#include <boost/shared_ptr.hpp>
#include <ClanLib/Core/Math/vec3.h>

#include <world/serverobject.hpp>

namespace fluo {
namespace world {

class SmoothMovement {
public:
    SmoothMovement(boost::shared_ptr<world::ServerObject> obj, CL_Vec3f diff, unsigned int durationMillis);

    bool wasStarted() const;
    bool isFinished() const;
    void start();
    void update(unsigned int elapsedMillis);

private:
    boost::shared_ptr<world::ServerObject> movingObject_;
    CL_Vec3f diff_;
    unsigned int duration_;

    CL_Vec3f startLoc_;
    unsigned int totalElapsedMillis_;
};

}
}

#endif
