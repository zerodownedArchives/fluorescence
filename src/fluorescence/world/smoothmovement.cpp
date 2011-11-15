
#include "smoothmovement.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace world {

SmoothMovement::SmoothMovement(boost::shared_ptr<world::ServerObject> obj, CL_Vec3f diff, unsigned int durationMillis) :
        movingObject_(obj), diff_(diff), duration_(durationMillis), wasStarted_(false) {
}

SmoothMovement::SmoothMovement(boost::shared_ptr<world::ServerObject> obj, uint8_t direction, unsigned int durationMillis) :
        movingObject_(obj), duration_(durationMillis), wasStarted_(false) {

    switch (direction & 0x7) {
        case Direction::N: diff_ = CL_Vec3f(0, -1, 0); break;
        case Direction::NE: diff_ = CL_Vec3f(1, -1, 0); break;
        case Direction::E: diff_ = CL_Vec3f(1, 0, 0); break;
        case Direction::SE: diff_ = CL_Vec3f(1, 1, 0); break;
        case Direction::S: diff_ = CL_Vec3f(0, 1, 0); break;
        case Direction::SW: diff_ = CL_Vec3f(-1, 1, 0); break;
        case Direction::W: diff_ = CL_Vec3f(-1, 0, 0); break;
        case Direction::NW: diff_ = CL_Vec3f(-1, -1, 0); break;
    }

    // TODO: get z level of the new tile
}

bool SmoothMovement::wasStarted() const {
    return wasStarted_;
}

bool SmoothMovement::isFinished() const {
    return totalElapsedMillis_ >= duration_;
}

void SmoothMovement::start() {
    wasStarted_ = true;
    totalElapsedMillis_ = 0;
    startLoc_ = movingObject_->getLocation();
}

void SmoothMovement::finish() {
    // necessary to set this to the precise target location. otherwise rounding errors might add up
    movingObject_->setLocation(startLoc_ + diff_);
}

void SmoothMovement::update(unsigned int elapsedMillis) {
    CL_Vec3f movementDelta = diff_ * ((float)elapsedMillis / duration_);

    CL_Vec3f curLoc = movingObject_->getLocation() + movementDelta;
    movingObject_->setLocation(curLoc);

    totalElapsedMillis_ += elapsedMillis;
}

}
}
