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



#include "7c_objectpicker.hpp"

#include <misc/log.hpp>
#include <ui/gumpmenus.hpp>

namespace fluo {
namespace net {
namespace packets {

ObjectPicker::ObjectPicker() : Packet(0x7c) {
}

bool ObjectPicker::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::read(buf, len, index, menuId_);

    uint8_t titleLength;
    ret &= PacketReader::read(buf, len, index, titleLength);
    if (titleLength > 0) {
        ret &= PacketReader::readUtf8Fixed(buf, len, index, title_, titleLength);
    }

    uint8_t itemCount;
    ret &= PacketReader::read(buf, len, index, itemCount);

    for (uint8_t i = 0; i < itemCount; ++i) {
        ObjectPickerItem curItem;
        curItem.answerId_ = i + 1;
        ret &= PacketReader::read(buf, len, index, curItem.artId_);
        ret &= PacketReader::read(buf, len, index, curItem.hue_);
        uint8_t textLength;
        ret &= PacketReader::read(buf, len, index, textLength);

        if (textLength > 0) {
            ret &= PacketReader::readUtf8Fixed(buf, len, index, curItem.text_, textLength);
        }

        items_.push_back(curItem);
    }

    return ret;
}

void ObjectPicker::onReceive() {
    if (items_.empty()) {
        LOG_INFO << "Received objectpicker without any options" << std::endl;
        return;
    } else {
        ui::GumpMenus::openObjectPicker(this);
    }
}

}
}
}
