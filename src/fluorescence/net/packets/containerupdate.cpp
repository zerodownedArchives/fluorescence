
#include "containerupdate.hpp"

#include <boost/shared_ptr.hpp>

#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

ContainerUpdate::ContainerUpdate() : Packet(0x25, 20) {
}

bool ContainerUpdate::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::read(buf, len, index, artId_);
    ret &= PacketReader::read(buf, len, index, stackIdOffset_);
    ret &= PacketReader::read(buf, len, index, amount_);
    ret &= PacketReader::read(buf, len, index, pxX_);
    ret &= PacketReader::read(buf, len, index, pxY_);

    //if (useGridByte) {
        //++index;
    //}

    ret &= PacketReader::read(buf, len, index, container_);
    ret &= PacketReader::read(buf, len, index, hue_);

    return ret;
}

void ContainerUpdate::onReceive() {
    boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(serial_);

    boost::shared_ptr<world::DynamicItem> cont = world::Manager::getSingleton()->getDynamicItem(container_, false);
    if (!cont) {
        LOG_WARN << "Received container update for unknown container " << container_ << std::endl;
        return;
    }

    itm->setArtId(artId_);
    itm->setStackIdOffset(stackIdOffset_);
    itm->setAmount(amount_);
    itm->setLocation(pxX_, pxY_, 0);
    itm->setHue(hue_);

    cont->addChildObject(itm);
}

}
}
}
