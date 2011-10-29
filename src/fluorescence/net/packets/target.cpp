
#include "target.hpp"

#include <boost/shared_ptr.hpp>

#include <world/mobile.hpp>
#include <world/dynamicitem.hpp>
#include <world/statics.hpp>
#include <world/manager.hpp>

#include <ui/manager.hpp>
#include <ui/cursormanager.hpp>
#include <ui/targeting/servertarget.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

Target::Target() : Packet(0x6c, 19), targetType_(0), flags_(0), targetSerial_(0), locX_(0xFFFF), locY_(0xFFFF), locZ_(0xFF), graphicId_(0) {
    senderSerial_ = world::Manager::getSingleton()->getPlayer()->getSerial();
}

Target::Target(boost::shared_ptr<world::IngameObject> obj) : Packet(0x6c, 19) {
    senderSerial_ = world::Manager::getSingleton()->getPlayer()->getSerial();
    flags_ = 0;
    locX_ = obj->getLocX();
    locY_ = obj->getLocY();
    locZ_ = obj->getLocZ();

    if (obj->isMobile()) {
        boost::shared_ptr<world::Mobile> mob = boost::dynamic_pointer_cast<world::Mobile>(obj);
        targetType_ = 0;
        targetSerial_ = mob->getSerial();
        graphicId_ = mob->getBaseBodyId();
    } else if (obj->isDynamicItem()) {
        boost::shared_ptr<world::DynamicItem> itm = boost::dynamic_pointer_cast<world::DynamicItem>(obj);
        targetType_ = 0;
        targetSerial_ = itm->getSerial();
        graphicId_ = itm->getArtId();
    } else if (obj->isStaticItem()) {
        boost::shared_ptr<world::StaticItem> itm = boost::dynamic_pointer_cast<world::StaticItem>(obj);
        targetType_ = 1;
        targetSerial_ = 0;
        graphicId_ = itm->getArtId();
    } else if (obj->isMap()) {
        targetType_ = 1;
        targetSerial_ = 0;
        graphicId_ = 0;
    } else {
        LOG_ERROR << "packets::Target for invalid IngameObject" << std::endl;
    }
}

bool Target::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    ret &= PacketWriter::write(buf, len, index, targetType_);
    ret &= PacketWriter::write(buf, len, index, senderSerial_);
    ret &= PacketWriter::write(buf, len, index, flags_);
    ret &= PacketWriter::write(buf, len, index, targetSerial_);
    ret &= PacketWriter::write(buf, len, index, locX_);
    ret &= PacketWriter::write(buf, len, index, locY_);
    ret &= PacketWriter::write(buf, len, index, locZ_);
    ret &= PacketWriter::write(buf, len, index, graphicId_);

    return ret;
}

bool Target::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, targetType_);
    ret &= PacketReader::read(buf, len, index, senderSerial_);
    ret &= PacketReader::read(buf, len, index, flags_);
    ret &= PacketReader::read(buf, len, index, targetSerial_);
    ret &= PacketReader::read(buf, len, index, locX_);
    ret &= PacketReader::read(buf, len, index, locY_);
    ret &= PacketReader::read(buf, len, index, locZ_);
    ret &= PacketReader::read(buf, len, index, graphicId_);

    return ret;
}

void Target::onReceive() {
    if (targetType_ == 0 && senderSerial_ == 0 && flags_ == 3) {
        // server-requested cancel
        // pol doc suggests to send a cancel packet for the old target here, but runuo does not seem to like that...
        // ui::Manager::getCursorManager()->cancelTarget();
    } else {
        boost::shared_ptr<ui::targeting::ServerTarget> targ(new ui::targeting::ServerTarget(targetType_ == 1));
        ui::Manager::getCursorManager()->setTarget(targ);
    }
}

}
}
}
