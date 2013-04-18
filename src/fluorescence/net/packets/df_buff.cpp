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



#include "df_buff.hpp"

#include <misc/log.hpp>
#include <data/manager.hpp>
#include <data/clilocloader.hpp>
#include <world/manager.hpp>
#include <world/mobile.hpp>

namespace fluo {
namespace net {
namespace packets {

Buff::Buff() : Packet(0xdf) {
}

bool Buff::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::read(buf, len, index, buffType_);
    buffInfo_.buffType_ = buffType_;

    ret &= PacketReader::read(buf, len, index, packetAction_);

    if (packetAction_ == 0) {
        // remove buff
        index += 4;
    } else if (packetAction_ == 1) {
        // add buff
        ret &= PacketReader::read(buf, len, index, buffInfo_.sourceType_);
        index += 2; // skip
        ret &= PacketReader::read(buf, len, index, buffInfo_.iconId_);
        index += 6; // skip
        ret &= PacketReader::read(buf, len, index, buffInfo_.duration_);
        index += 3; // skip
        ret &= PacketReader::read(buf, len, index, buffInfo_.clilocTitle_);
        ret &= PacketReader::read(buf, len, index, buffInfo_.cliloc2_);
        ret &= PacketReader::read(buf, len, index, buffInfo_.cliloc3_);

#if 1
        // runuo sends some weird stuff here...
        // this will most likely not work on any other server emulator
        uint16_t moreData;
        ret &= PacketReader::read(buf, len, index, moreData);
        if (moreData == 0) {
            index += 4;
        } else {
            index += 2;
            // why would you send a tab as the first char of cliloc args?
            index += 2;
            ret &= PacketReader::readUnicodeNullLE(buf, len, index, buffInfo_.cliloc2Args_);

            // more garbage
            index += 4;
        }
#else
        // this would be the code according to wayne's packet doc
        // however, the above code works with runuo... maybe adapt this in a future version
        uint16_t strlen;
        ret &= PacketReader::read(buf, len, index, strlen);
        if (strlen > 0) {
            ret &= PacketReader::readUnicodeFixed(buf, len, index, buffInfo_.clilocTitleArgs_, strlen);
        }
        ret &= PacketReader::read(buf, len, index, strlen);
        if (strlen > 0) {
            ret &= PacketReader::readUnicodeFixed(buf, len, index, buffInfo_.cliloc2Args_, strlen);
        }
        ret &= PacketReader::read(buf, len, index, strlen);
        if (strlen > 0) {
            ret &= PacketReader::readUnicodeFixed(buf, len, index, buffInfo_.cliloc3Args_, strlen);
        }
#endif
    } else {
        LOG_WARN << "Uknown action in buff packet: " << packetAction_ << std::endl;
    }

    return ret;
}

void Buff::onReceive() {
    boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(serial_, false);
    if (!mob) {
        LOG_DEBUG << "Received Buff packet for unknown mobile " << serial_ << std::endl;
        return;
    }

    if (packetAction_ == 0) {
        // remove buff
        mob->removeBuff(buffType_);
    } else if (packetAction_ == 1) {
        mob->addBuff(buffInfo_);
    }
}

}
}
}
