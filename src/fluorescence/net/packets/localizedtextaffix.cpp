
#include "localizedtextaffix.hpp"

#include <data/manager.hpp>
#include <data/clilocloader.hpp>

namespace fluo {
namespace net {
namespace packets {

LocalizedTextAffix::LocalizedTextAffix() : BaseText(0xcc) {
}

bool LocalizedTextAffix::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret = ret && PacketReader::read(buf, len, index, serial_);
    ret = ret && PacketReader::read(buf, len, index, bodyId_);
    ret = ret && PacketReader::read(buf, len, index, type_);
    ret = ret && PacketReader::read(buf, len, index, hue_);
    ret = ret && PacketReader::read(buf, len, index, font_);
    ret = ret && PacketReader::read(buf, len, index, clilocId_);
    ret = ret && PacketReader::read(buf, len, index, affixType_);
    ret = ret && PacketReader::readUtf8Fixed(buf, len, index, speaker_, 30);
    ret = ret && PacketReader::readUtf8Null(buf, len, index, affix_);
    ret = ret && PacketReader::readUnicodeNull(buf, len, index, arguments_);


    text_ = data::Manager::getClilocLoader()->get(clilocId_, arguments_);

    if (affixType_ & 0x1) {
        // prepend
        text_ = affix_ + text_;
    } else {
        text_ = text_ + affix_;
    }

    //LOG_DEBUG << "Received localized text with cliloc=" << clilocId_ << " affix=" << affix_ << " arguments=" << arguments_ << std::endl;
    //LOG_DEBUG << "Translates to: " << text_ << std::endl;

    return ret;
}

}
}
}
