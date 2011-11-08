
#include "localizedtext.hpp"

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

    LOG_DEBUG << "Received localized text with cliloc=" << clilocId_ << " arguments=" << arguments_ << std::endl;
    LOG_DEBUG << "Translates to: " << text_ << std::endl;

    return ret;
}

}
}
}
