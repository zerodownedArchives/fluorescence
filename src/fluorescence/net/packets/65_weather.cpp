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



#include "65_weather.hpp"

#include <world/manager.hpp>

namespace fluo {
namespace net {
namespace packets {

Weather::Weather() : Packet(0x65, 4) {
}

bool Weather::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, type_);
    ret &= PacketReader::read(buf, len, index, intensity_);
    ret &= PacketReader::read(buf, len, index, temperature_);

    return ret;
}

void Weather::onReceive() {
    world::Manager::getSingleton()->setWeather(type_, intensity_, temperature_);
}

}
}
}
