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



#include "c1_localizedtext.hpp"

#include <data/manager.hpp>
#include <data/clilocloader.hpp>

namespace fluo {
namespace net {
namespace packets {

LocalizedText::LocalizedText() : BaseText(0x1c) {
}

bool LocalizedText::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret = ret && PacketReader::read(buf, len, index, serial_);
    ret = ret && PacketReader::read(buf, len, index, bodyId_);
    ret = ret && PacketReader::read(buf, len, index, type_);
    ret = ret && PacketReader::read(buf, len, index, hue_);
    ret = ret && PacketReader::read(buf, len, index, font_);
    ret = ret && PacketReader::read(buf, len, index, clilocId_);
    ret = ret && PacketReader::readUtf8Fixed(buf, len, index, speaker_, 30);
    ret = ret && PacketReader::readUnicodeNullLE(buf, len, index, arguments_);

    text_ = data::Manager::getClilocLoader()->get(clilocId_, arguments_);

    //LOG_DEBUG << "Received localized text with cliloc=" << clilocId_ << " arguments=" << arguments_ << std::endl;
    //LOG_DEBUG << "Translates to: " << text_ << std::endl;

    return ret;
}

}
}
}
