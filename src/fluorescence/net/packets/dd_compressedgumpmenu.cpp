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



#include "dd_compressedgumpmenu.hpp"

#include <zlib.h>

#include <misc/log.hpp>
#include <ui/stringparser.hpp>
#include <ui/gumpmenu.hpp>

namespace fluo {
namespace net {
namespace packets {

CompressedGumpMenu::CompressedGumpMenu() : Packet(0xDD) {
}

bool CompressedGumpMenu::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, gumpSerial_);
    ret &= PacketReader::read(buf, len, index, typeId_);
    ret &= PacketReader::read(buf, len, index, locX_);
    ret &= PacketReader::read(buf, len, index, locY_);

    uint32_t compressedLength = 0;
    ret &= PacketReader::read(buf, len, index, compressedLength);
    // compressed data length is 4 bytes more than the actual data
    compressedLength -= 4;

    uint32_t decompressedLength = 0;
    ret &= PacketReader::read(buf, len, index, decompressedLength);

    unsigned long int realDecompressedLength = decompressedLength;

    unsigned char* decompressedBuffer = (unsigned char*)malloc(decompressedLength);
    // let zlib do the work :)
    int zlibRet = uncompress(decompressedBuffer, &realDecompressedLength, (const unsigned char*)&buf[index], compressedLength);

    if (zlibRet != Z_OK || realDecompressedLength != decompressedLength) {
        LOG_ERROR << "Error decompressing a gump menu. err=" << zlibRet << " len=" << realDecompressedLength << " expected=" << decompressedLength << std::endl;
        free(decompressedBuffer);
        return false;
    }

    index += compressedLength;

    gumpData_ = StringConverter::fromUtf8((const char*)decompressedBuffer, decompressedLength);


    uint32_t textLineCount;
    ret &= PacketReader::read(buf, len, index, textLineCount);

    // check if there are any strings
    if (textLineCount == 0) {
        index += 4; // jump compressed length
        return ret;
    }

    textLines_.resize(textLineCount);

    ret &= PacketReader::read(buf, len, index, compressedLength);
    // compressed data length is 4 bytes more than the actual data
    compressedLength -= 4;

    ret &= PacketReader::read(buf, len, index, decompressedLength);

    // reuse buffer if it fits
    // realDecompressedLength still holds the last buffer size
    if (realDecompressedLength < decompressedLength) {
        decompressedBuffer = (unsigned char*)realloc(decompressedBuffer, decompressedLength);
    }

    realDecompressedLength = decompressedLength;

    zlibRet = uncompress(decompressedBuffer, &realDecompressedLength, (const unsigned char*)&buf[index], compressedLength);

    if (zlibRet != Z_OK || realDecompressedLength != decompressedLength) {
        LOG_ERROR << "Error decompressing strings of a gump menu. err=" << zlibRet << " len=" << realDecompressedLength << " expected=" << decompressedLength << std::endl;
        free(decompressedBuffer);
        return false;
    }

    index += compressedLength;

    unsigned int decompressedIndex = 0;
    uint16_t dataLength;
    for (unsigned int i = 0; i < textLineCount; ++i) {
        // read strings from decompressed buffer instead of the packet stream
        ret &= PacketReader::read((const int8_t*)decompressedBuffer, decompressedLength, decompressedIndex, dataLength);
        ret &= PacketReader::readUnicodeFixed((const int8_t*)decompressedBuffer, decompressedLength, decompressedIndex, textLines_[i], dataLength);
    }

    free(decompressedBuffer);

    return ret;
}

void CompressedGumpMenu::onReceive() {
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
