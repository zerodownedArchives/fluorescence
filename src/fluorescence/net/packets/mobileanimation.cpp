
#include "mobileanimation.hpp"

#include <world/mobile.hpp>
#include <world/manager.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

MobileAnimation::MobileAnimation() : Packet(0x6e, 14) {
}

bool MobileAnimation::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret = ret && PacketReader::read(buf, len, index, serial_);
    ret = ret && PacketReader::read(buf, len, index, actionId_);
    ret = ret && PacketReader::read(buf, len, index, frameCount_);
    ret = ret && PacketReader::read(buf, len, index, repeatTimes_);
    ret = ret && PacketReader::read(buf, len, index, forward_);
    ret = ret && PacketReader::read(buf, len, index, repeat_);
    ret = ret && PacketReader::read(buf, len, index, delay_);

    return ret;
}

void MobileAnimation::onReceive() {
    if (repeatTimes_ > 1 || forward_ != 0 || repeat_ != 0 || delay_ > 0) {
        // TODO: implement this
        LOG_WARN << "Mobile animation support is only rudimentary, at least one used parameter is not really supported at the moment" << std::endl;
        LOG_WARN << "Mobile animation action=" << actionId_ << " frameCount=" << frameCount_ << " repeatTimes=" << repeatTimes_
                << " forward=" << (unsigned int)forward_ << " repeat=" << (unsigned int)repeat_ << " delay=" << (unsigned int)delay_ << std::endl;
    }

    boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(serial_, false);

    if (mob) {
        mob->playAnim(actionId_);
    }
}

}
}
}
