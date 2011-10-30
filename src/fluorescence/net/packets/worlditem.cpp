
#include "worlditem.hpp"

#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

#include <ui/manager.hpp>

namespace fluo {
namespace net {
namespace packets {

WorldItem::WorldItem() : Packet(0x1A) {
}

bool WorldItem::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    bool readAmount = false;
    bool readStackIdOffset = false;
    bool readDirection = false;
    bool readHue = false;
    bool readStatus = false;

    ret = ret && PacketReader::read(buf, len, index, serial_);
    if (serial_ & 0x80000000) {
        readAmount = true;
        serial_ &= ~0x80000000;
    }

    ret = ret && PacketReader::read(buf, len, index, artId_);
    if (artId_ & 0x8000) {
        readStackIdOffset = true;
        artId_ &= ~0x8000;
    }

    if (readAmount) {
        ret = ret && PacketReader::read(buf, len, index, amount_);
    } else {
        amount_ = 1;
    }

    if (readStackIdOffset) {
        ret = ret && PacketReader::read(buf, len, index, stackIdOffset_);
    } else {
        stackIdOffset_ = 0;
    }

    ret = ret && PacketReader::read(buf, len, index, locX_);
    if (locX_ & 0x8000) {
        readDirection = true;
        locX_ &= ~0x8000;
    }

    ret = ret && PacketReader::read(buf, len, index, locY_);
    if (locY_ & 0x8000) {
        readHue = true;
        locY_ &= ~0x8000;
    }
    if (locY_ & 0x4000) {
        readStatus = true;
        locY_ &= ~0x4000;
    }

    if (readDirection) {
        ret = ret && PacketReader::read(buf, len, index, direction_);
    } else {
        direction_ = 0;
    }

    ret = ret && PacketReader::read(buf, len, index, locZ_);

    if (readHue) {
        ret = ret && PacketReader::read(buf, len, index, hue_);
    } else {
        hue_ = 0;
    }

    if (readStatus) {
        ret = ret && PacketReader::read(buf, len, index, status_);
    } else {
        status_ = 0;
    }

    return ret;
}

void WorldItem::onReceive() {
    boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(serial_);

    itm->setLocation(locX_, locY_, locZ_);
    itm->setDirection(direction_);
    itm->setAmount(amount_);
    itm->setStackIdOffset(stackIdOffset_);
    itm->setArtId(artId_);
    itm->setHue(hue_);

    // TODO: handle status

    itm->addToRenderQueue(ui::Manager::getWorldRenderQueue());
}

}
}
}
