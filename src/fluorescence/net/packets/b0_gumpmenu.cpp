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



#include "b0_gumpmenu.hpp"

#include <misc/log.hpp>
#include <ui/stringparser.hpp>
#include <ui/gumpmenu.hpp>

namespace fluo {
namespace net {
namespace packets {

GumpMenu::GumpMenu() : Packet(0xB0) {
}

bool GumpMenu::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, gumpSerial_);
    ret &= PacketReader::read(buf, len, index, typeId_);
    ret &= PacketReader::read(buf, len, index, locX_);
    ret &= PacketReader::read(buf, len, index, locY_);

    uint16_t dataLength = 0;
    ret &= PacketReader::read(buf, len, index, dataLength);
    ret &= PacketReader::readUtf8Fixed(buf, len, index, gumpData_, dataLength);

    uint16_t textLineCount;
    ret &= PacketReader::read(buf, len, index, textLineCount);
    textLines_.resize(textLineCount);
    for (unsigned int i = 0; i < textLineCount; ++i) {
        ret &= PacketReader::read(buf, len, index, dataLength);
        ret &= PacketReader::readUnicodeFixed(buf, len, index, textLines_[i], dataLength);
    }

    return ret;
}

void GumpMenu::onReceive() {
    ui::GumpMenu* menu = ui::StringParser::fromString(gumpData_, textLines_);
    menu->setSerial(gumpSerial_);
    menu->setTypeId(typeId_);
    CL_Rectf geom = menu->get_geometry();
    geom.translate(locX_, locY_);
    menu->set_geometry(geom);
}

}
}
}
