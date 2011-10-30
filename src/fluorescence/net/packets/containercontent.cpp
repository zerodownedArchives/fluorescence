
#include "containercontent.hpp"

#include <boost/shared_ptr.hpp>

#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

ContainerContent::ContainerContent() : Packet(0x3C) {
}

bool ContainerContent::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    uint16_t itemCount;
    ret &= PacketReader::read(buf, len, index, itemCount);

    bool useGridByte = (len-5 == itemCount* 20);

    for (unsigned int i = 0; i < itemCount; ++i) {
        ContainerContentEntry curEntry;

        ret &= PacketReader::read(buf, len, index, curEntry.serial_);
        ret &= PacketReader::read(buf, len, index, curEntry.artId_);
        ret &= PacketReader::read(buf, len, index, curEntry.stackIdOffset_);
        ret &= PacketReader::read(buf, len, index, curEntry.amount_);
        ret &= PacketReader::read(buf, len, index, curEntry.pxX_);
        ret &= PacketReader::read(buf, len, index, curEntry.pxY_);

        if (useGridByte) {
            // jump, for now
            ++index;
        }

        ret &= PacketReader::read(buf, len, index, curEntry.container_);
        ret &= PacketReader::read(buf, len, index, curEntry.hue_);

        entries_.push_back(curEntry);
    }

    return ret;
}

void ContainerContent::onReceive() {
    std::list<ContainerContentEntry>::const_iterator iter = entries_.begin();
    std::list<ContainerContentEntry>::const_iterator end = entries_.end();

    for (; iter != end; ++iter) {
        boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(iter->serial_);

        // why is the container serial here for each item? is it possible to add items to several containers with this packet?
        // to be safe, get container for each item entry
        boost::shared_ptr<world::DynamicItem> cont = world::Manager::getSingleton()->getDynamicItem(iter->container_, false);
        if (!cont) {
            LOG_WARN << "Received container contents for unknown container " << iter->container_ << std::endl;
            continue;
        }

        itm->setArtId(iter->artId_);
        itm->setStackIdOffset(iter->stackIdOffset_);
        itm->setAmount(iter->amount_);
        itm->setLocation(iter->pxX_, iter->pxY_, 0);
        itm->setHue(iter->hue_);

        cont->addChildObject(itm);
    }
}

}
}
}
