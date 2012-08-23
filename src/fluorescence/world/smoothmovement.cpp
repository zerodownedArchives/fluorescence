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



#include "smoothmovement.hpp"

#include "mobile.hpp"
#include "dynamicitem.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace world {

SmoothMovement::SmoothMovement(boost::shared_ptr<world::ServerObject> obj, CL_Vec3f targetLoc, unsigned int durationMillis) :
        movingObject_(obj), targetLoc_(targetLoc), duration_(durationMillis), wasStarted_(false), isDirectionChange_(false) {
}

SmoothMovement::SmoothMovement(boost::shared_ptr<world::ServerObject> obj, unsigned int direction, CL_Vec3f targetLoc) :
        movingObject_(obj), targetLoc_(targetLoc), wasStarted_(false), isDirectionChange_(true), newDirection_(direction) {
}

bool SmoothMovement::wasStarted() const {
    return wasStarted_;
}

bool SmoothMovement::isFinished() const {
    return totalElapsedMillis_ >= duration_ || isDirectionChange_;
}

void SmoothMovement::start() {
    wasStarted_ = true;
    totalElapsedMillis_ = 0;
    if (isDirectionChange_) {
        if (movingObject_->isMobile()) {
            boost::static_pointer_cast<world::Mobile>(movingObject_)->setDirection(newDirection_);
        } else if (movingObject_->isDynamicItem()) {
            boost::static_pointer_cast<world::DynamicItem>(movingObject_)->setDirection(newDirection_);
        }
    } else {
        startLoc_ = movingObject_->getLocation();
        diff_ = targetLoc_ - startLoc_;
    }

    if (startCallback_) {
        startCallback_();
    }
}

void SmoothMovement::finish(bool interrupted) {
    // necessary to set this to the precise target location. otherwise rounding errors might add up
    if (!interrupted && !isDirectionChange_) {
        movingObject_->setLocation(targetLoc_);
    }

    if (finishedCallback_) {
        finishedCallback_();
    }
}

unsigned int SmoothMovement::update(unsigned int elapsedMillis) {
    if (isDirectionChange_) {
        return 0;
    }

    unsigned int usedMillis = elapsedMillis;
    if (totalElapsedMillis_ + elapsedMillis >= duration_) {
        usedMillis = duration_ - totalElapsedMillis_;
    }

    CL_Vec3f movementDelta = diff_ * ((float)usedMillis / duration_);

    CL_Vec3f curLoc = movingObject_->getLocation() + movementDelta;
    movingObject_->setLocation(curLoc);

    totalElapsedMillis_ += usedMillis;

    return elapsedMillis - usedMillis;
}

void SmoothMovement::setFinishedCallback(FinishedCallback cb) {
    finishedCallback_ = cb;
}

void SmoothMovement::setStartCallback(StartCallback cb) {
    startCallback_ = cb;
}

CL_Vec3f SmoothMovement::getTargetLoc() const {
    return targetLoc_;
}

}
}
