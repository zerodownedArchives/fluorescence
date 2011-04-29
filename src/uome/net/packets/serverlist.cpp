
#include "serverlist.hpp"

#include <misc/logger.hpp>

#include <ui/manager.hpp>
#include <ui/gumpmenus.hpp>

namespace uome {
namespace net {
namespace packets {

ServerList::ServerList() : Packet(0xA8) {
}

bool ServerList::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret = ret && PacketReader::read(buf, len, index, flags_);

    uint16_t listSize;
    ret = ret && PacketReader::read(buf, len, index, listSize);

    for (unsigned int i = 0; i < listSize && ret; ++i) {
        ServerListEntry entry;
        ret = ret && PacketReader::read(buf, len, index, entry.index_);
        ret = ret && PacketReader::readUtf8Fixed(buf, len, index, entry.name_, 32);
        ret = ret && PacketReader::read(buf, len, index, entry.fullPercentage_);
        ret = ret && PacketReader::read(buf, len, index, entry.timezoneBias_);
        ret = ret && PacketReader::read(buf, len, index, entry.serverAddress_);

        listEntries_.push_back(entry);
    }

    return ret;
}

void ServerList::onReceive() {
    ui::Manager::getSingleton()->closeGumpMenu("login");
    ui::GumpMenus::openServerListGump(this);
}

}
}
}
