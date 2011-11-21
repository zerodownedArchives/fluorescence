
#include "smoothmovementmanager.hpp"

#include <boost/bind.hpp>

namespace fluo {
namespace world {

void SmoothMovementManager::update(unsigned int elapsedMillis) {
    std::map<Serial, std::list<SmoothMovement> >::iterator iter = movementQueues_.begin();
    std::map<Serial, std::list<SmoothMovement> >::iterator end = movementQueues_.end();

    while (iter != end) {
        if (iter->second.empty()) {
            movementQueues_.erase(iter++);
            continue;
        }

        SmoothMovement* curMov = &iter->second.front();
        if (!curMov->wasStarted()) {
            curMov->start();
        }

        if (curMov->isFinished()) {
            curMov->finish();
            iter->second.pop_front();

            if (iter->second.empty()) {
                movementQueues_.erase(iter++);
                continue;
            } else {
                curMov = &iter->second.front();
                curMov->start();
            }
        }

        curMov->update(elapsedMillis);
        ++iter;
    }
}

void SmoothMovementManager::add(Serial serial, SmoothMovement& movement) {
    movementQueues_[serial].push_back(movement);
}

void SmoothMovementManager::clear(Serial serial) {
    movementQueues_.erase(serial);
}

}
}