
#include "smoothmovement.hpp"


namespace fluo {
namespace world {

SmoothMovement::SmoothMovement(boost::shared_ptr<world::ServerObject> obj, CL_Vec3f diff, unsigned int durationMillis) :
        movingObject_(obj), diff_(diff), duration_(durationMillis) {
}

bool SmoothMovement::wasStarted() const {
    return startLoc_ != CL_Vec3f(0, 0, 0);
}

bool SmoothMovement::isFinished() const {
    return totalElapsedMillis_ >= duration_;
}

void SmoothMovement::start() {
    totalElapsedMillis_ = 0;
    startLoc_ = movingObject_->getLocation();
}

void SmoothMovement::update(unsigned int elapsedMillis) {
    CL_Vec3f movementDelta = diff_ / (elapsedMillis / 1000);

    CL_Vec3f curLoc = startLoc_ + movementDelta;
    movingObject_->setLocation(curLoc);

    totalElapsedMillis_ += elapsedMillis;
}

}
}
