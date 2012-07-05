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



#include "1b_spellbookcontent.hpp"

#include <misc/log.hpp>
#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

namespace fluo {
namespace net {
namespace packets {
namespace bf {

SpellbookContent::SpellbookContent() : Packet(0x1B) {
}

bool SpellbookContent::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    index += 2; // jump 0x1

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::read(buf, len, index, graphicId_);
    ret &= PacketReader::read(buf, len, index, offset_);

    for (unsigned int i = 0; i < 8; ++i) {
        ret &= PacketReader::read(buf, len, index, spellBits_[i]);
    }

    return ret;
}

void SpellbookContent::onReceive() {
    boost::shared_ptr<world::DynamicItem> dynItm = world::Manager::getSingleton()->getDynamicItem(serial_, false);
    if (dynItm) {
        dynItm->setSpellbook(offset_, spellBits_);
    } else {
        LOG_WARN << "Received spellbook content for nonexistant item serial " << serial_ << std::endl;
    }
}

}
}
}
}
