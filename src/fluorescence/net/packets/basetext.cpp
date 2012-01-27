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



#include "basetext.hpp"

#include <misc/log.hpp>

#include <world/manager.hpp>
#include <world/mobile.hpp>
#include <world/dynamicitem.hpp>
#include <world/overheadmessage.hpp>

#include <ui/manager.hpp>

namespace fluo {
namespace net {
namespace packets {

BaseText::BaseText(uint8_t packetId) : Packet(packetId) {
}

void BaseText::onReceive() {
    if (type_ == TextType::SYSTEM || serial_ == 0xFFFFFFFF) {
        ui::Manager::getSingleton()->systemMessage(text_);
    } else if (mobileMessage()) {
    } else if (itemMessage()) {
    } else {
        LOG_WARN << "Speech packet from " << speaker_ << ": unable to find serial - " << text_ << std::endl;
    }
}

bool BaseText::mobileMessage() {
    boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(serial_, false);

    if (mob) {
        mob->setBodyId(bodyId_);
        mob->getProperty("name").setString(speaker_);
        mob->onPropertyUpdate();

        boost::shared_ptr<world::OverheadMessage> msg(new world::OverheadMessage(text_, 1, 5, false));
        world::Manager::getSingleton()->registerOverheadMessage(msg);
        mob->addChildObject(msg);
        LOG_INFO << "Speech from " << speaker_ << ": " << text_ << std::endl;
    }

    return (bool)mob;
}

bool BaseText::itemMessage() {
    boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(serial_, false);

    if (itm) {
        boost::shared_ptr<world::OverheadMessage> msg(new world::OverheadMessage(text_, 1, 5, false));
        world::Manager::getSingleton()->registerOverheadMessage(msg);
        itm->addChildObject(msg);
        LOG_INFO << "Overhead text for " << speaker_ << ": " << text_ << std::endl;
    }

    return (bool)itm;
}

}
}
}
