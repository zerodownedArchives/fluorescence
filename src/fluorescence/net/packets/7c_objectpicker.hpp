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


#ifndef FLUO_NET_PACKETS_OBJECTPICKER_HPP
#define FLUO_NET_PACKETS_OBJECTPICKER_HPP

#include <net/packet.hpp>

#include <vector>

namespace fluo {

namespace ui {
class GumpMenus;
}

namespace net {
namespace packets {

struct ObjectPickerItem {
    uint16_t answerId_;
    uint16_t artId_;
    uint16_t hue_;
    UnicodeString text_;
};

class ObjectPicker : public Packet {

friend class ui::GumpMenus;

public:
    ObjectPicker();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

private:
    uint32_t serial_;
    uint16_t menuId_;
    UnicodeString title_;

    std::vector<ObjectPickerItem> items_;
};

}
}
}

#endif
