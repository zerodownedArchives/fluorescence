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


#ifndef FLUO_NET_PACKET_HPP
#define FLUO_NET_PACKET_HPP

#include <stdint.h>

#include "packetwriter.hpp"
#include "packetreader.hpp"

namespace fluo {
namespace net {

class Packet {
public:
    // 0 indicates a variable sized packet
    Packet(uint8_t id, uint16_t size = 0);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;
    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    bool hasVariableSize();
    uint16_t getSize(); ///< for fixed size packets
    void setSize(uint16_t val);

    virtual void onReceive();

    uint8_t getId();

protected:
    virtual bool writePacketId(int8_t* buf, unsigned int len, unsigned int& index) const;
    virtual bool writeSubPacketId(int8_t* buf, unsigned int len, unsigned int& index) const;

    unsigned int preparePacketSize(unsigned int& index) const;
    bool writePacketSize(int8_t* buf, unsigned int len, unsigned int& index, unsigned int sizeIndex) const;

    uint8_t id_;
    uint16_t size_;
};

}
}

#endif
