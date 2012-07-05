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



#include "ef_seed.hpp"

namespace fluo {
namespace net {
namespace packets {

Seed::Seed(unsigned int seed, unsigned int major, unsigned int minor, unsigned int revision, unsigned int build) : Packet(0xEF, 21),
        seed_(seed), versionMajor_(major), versionMinor_(minor), versionRevision_(revision), versionBuild_(build) {
}

bool Seed::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);

    ret &= PacketWriter::write(buf, len, index, seed_);
    ret &= PacketWriter::write(buf, len, index, versionMajor_);
    ret &= PacketWriter::write(buf, len, index, versionMinor_);
    ret &= PacketWriter::write(buf, len, index, versionRevision_);
    ret &= PacketWriter::write(buf, len, index, versionBuild_);

    return ret;
}

}
}
}
