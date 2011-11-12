#ifndef FLUO_NET_WALKMANAGER_HPP
#define FLUO_NET_WALKMANAGER_HPP

#include <boost/shared_ptr.hpp>
#include <stdint.h>

namespace fluo {

namespace world {
class Mobile;
}

namespace net {

namespace packets {
class MovementDeny;
}

class WalkManager {
public:
    WalkManager();

    void initFastWalkStack(const uint32_t* values);
    void updateFastWalkStack(uint32_t value);

    void onMovementAccept(uint8_t sequence);
    void onMovementDeny(const net::packets::MovementDeny* pkt);
    void onMovementRequest(uint8_t direction);

private:
    boost::shared_ptr<world::Mobile> player_;

    uint32_t fastWalkStack_[6];
    unsigned int fastWalkStackIdx_;
    uint8_t curSequence_;

    unsigned int unacceptedSequenceCount_;
    uint8_t unacceptedSequences_[10];
    static const unsigned int MAX_UNACCEPTED_SEQUENCE_COUNT = 4;
};

}
}

#endif
