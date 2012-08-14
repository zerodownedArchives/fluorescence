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



#include "bf.hpp"

#include "bf/01_fastwalkinit.hpp"
#include "bf/02_fastwalkupdate.hpp"
#include "bf/04_closegump.hpp"
#include "bf/08_changemap.hpp"
#include "bf/14_opencontextmenu.hpp"
#include "bf/1b_spellbookcontent.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

BF::BF() : MultiPurposePacket(0xBF) {
}

BF::BF(boost::shared_ptr<Packet> subpacket) : MultiPurposePacket(0xBF, subpacket) {
}

boost::shared_ptr<Packet> BF::getSubpacket(uint16_t subId) {
    //LOG_DEBUG << "BF::getSubpacket for id " << (unsigned int)subId << std::endl;
    boost::shared_ptr<Packet> ret;

    switch (subId) {
    case 0x01: ret.reset(new bf::FastWalkInit()); break;
    case 0x02: ret.reset(new bf::FastWalkUpdate()); break;
    case 0x04: ret.reset(new bf::CloseGump()); break;
    case 0x08: ret.reset(new bf::ChangeMap()); break;
    case 0x14: ret.reset(new bf::OpenContextMenu()); break;
    case 0x1B: ret.reset(new bf::SpellbookContent()); break;
    }

    return ret;
}

}
}
}
