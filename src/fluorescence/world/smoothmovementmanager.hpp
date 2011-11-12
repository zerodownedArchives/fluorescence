#ifndef FLUO_WORLD_SMOOTHMOVEMENTMANAGER_HPP
#define FLUO_WORLD_SMOOTHMOVEMENTMANAGER_HPP

#include <map>
#include <list>

#include <typedefs.hpp>

#include "smoothmovement.hpp"

namespace fluo {
namespace world {

class SmoothMovementManager {
public:
    void update(unsigned int elapsedMillis);

    void add(Serial serial, SmoothMovement& movement);
    void clear(Serial serial);

private:
    std::map<Serial, std::list<SmoothMovement> > movementQueues_;
};

}
}

#endif
