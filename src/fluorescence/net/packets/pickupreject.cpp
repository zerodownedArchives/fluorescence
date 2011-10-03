
#include "pickupreject.hpp"

#include <ui/manager.hpp>
#include <ui/cursormanager.hpp>

namespace fluo {
namespace net {
namespace packets {

PickUpReject::PickUpReject() : Packet(0x27, 2) {
}

bool PickUpReject::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = PacketReader::read(buf, len, index, reason_);

    return ret;
}

void PickUpReject::onReceive() {
    ui::Manager* uiMan = ui::Manager::getSingleton();

    switch (reason_) {
        case 0x00: uiMan->systemMessage("You cannot pick that up."); break;
        case 0x01: uiMan->systemMessage("That is too far away."); break;
        case 0x02: uiMan->systemMessage("That is out of sight."); break;
        case 0x03: uiMan->systemMessage("That item does not belong to you. You will have to steal it."); break;
        case 0x04: uiMan->systemMessage("You are already holding an item."); break;
    }

    uiMan->getCursorManager()->stopDragging();
}

}
}
}
