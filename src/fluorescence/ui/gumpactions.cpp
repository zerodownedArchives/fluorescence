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



#include "gumpactions.hpp"

#include <boost/bind.hpp>

#include <client.hpp>

#include <ui/manager.hpp>
#include <ui/commandmanager.hpp>
#include <ui/components/lineedit.hpp>
#include <ui/components/basebutton.hpp>

#include <net/manager.hpp>
#include <net/packets/speechrequest.hpp>
#include <net/packets/createcharacter.hpp>
#include <net/packets/helprequest.hpp>
#include <net/packets/bf.hpp>
#include <net/packets/bf/contextmenureply.hpp>

#include <misc/log.hpp>

#include <world/mobile.hpp>


namespace fluo {
namespace ui {

GumpActions::GumpAction::GumpAction() {
}

GumpActions::GumpAction::GumpAction(bool closeGumpMenu, GumpActionCallback callback) : closeGumpMenu_(closeGumpMenu), callback_(callback) {
}

std::map<UnicodeString, GumpActions::GumpAction> GumpActions::actionTable_ = std::map<UnicodeString, GumpActions::GumpAction>();

void GumpActions::buildBasicActionTable() {
    actionTable_["shutdown"] = GumpAction(false, boost::bind(&Client::shutdown, Client::getSingleton(), _1, _2, _3, _4));
    actionTable_["selectshard"] = GumpAction(true, boost::bind(&Client::selectShard, Client::getSingleton(), _1, _2, _3, _4));
    actionTable_["selectshard-first"] = GumpAction(true, boost::bind(&GumpActions::selectShardFirst, _1, _2, _3, _4));
    actionTable_["close"] = GumpAction(true, boost::bind(&GumpActions::closeHelper, _1, _2, _3, _4));
}

void GumpActions::buildFullActionTable() {
    actionTable_["connect"] = GumpAction(false, boost::bind(&net::Manager::connect, net::Manager::getSingleton(), _1, _2, _3, _4));
    actionTable_["disconnect"] = GumpAction(true, boost::bind(&Client::disconnect, Client::getSingleton(), _1, _2, _3, _4));
    actionTable_["selectserver"] = GumpAction(false, boost::bind(&net::Manager::selectServer, net::Manager::getSingleton(), _1, _2, _3, _4));
    actionTable_["selectserver-first"] = GumpAction(true, boost::bind(&GumpActions::selectServerFirst, _1, _2, _3, _4));
    actionTable_["selectcharacter"] = GumpAction(true, boost::bind(&Client::selectCharacter, Client::getSingleton(), _1, _2, _3, _4));
    actionTable_["selectcharacter-first"] = GumpAction(true, boost::bind(&GumpActions::selectCharacterFirst, _1, _2, _3, _4));

    actionTable_["sendspeech"] = GumpAction(false, boost::bind(&GumpActions::sendSpeech, _1, _2, _3, _4));
    actionTable_["contextmenureply"] = GumpAction(true, boost::bind(&GumpActions::contextMenuReply, _1, _2, _3, _4));
    
    actionTable_["createdummychar"] = GumpAction(true, boost::bind(&GumpActions::createDummyCharacter, _1, _2, _3, _4));
    
    actionTable_["openstatus"] = GumpAction(false, boost::bind(&GumpActions::openStatus, _1, _2, _3, _4));
    actionTable_["helprequest"] = GumpAction(false, boost::bind(&GumpActions::helpRequest, _1, _2, _3, _4));
}


void GumpActions::doAction(GumpMenu* gump, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    std::map<UnicodeString, GumpAction>::const_iterator it = actionTable_.find(action);

    if (it != actionTable_.end()) {
        bool funcRet = (it->second.callback_) (gump, action, parameterCount, parameters);
        if (funcRet && it->second.closeGumpMenu_) { // close gump if callback was successful
            ui::Manager::getSingleton()->closeGumpMenu(gump);
        }
    } else {
        LOG_ERROR << "Unknown gump action: " << action << std::endl;
    }
}

void GumpActions::doAction(components::BaseButton* button) {
    GumpMenu* gump = button->getTopLevelMenu();
    if (gump) {
        doAction(gump, button->getAction(), components::BaseButton::MAX_PARAMETER_COUNT, button->getParameterPtr());
    } else {
        LOG_ERROR << "BaseButton inside something other than GumpMenu" << std::endl;
    }
}


bool GumpActions::closeHelper(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    return true;
}

bool GumpActions::selectShardFirst(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    components::BaseButton* button = dynamic_cast<components::BaseButton*>(menu->get_named_item("selectshard"));
    if (!button) {
        LOG_DEBUG << "No shard found for quickselect" << std::endl;
    } else {
        doAction(button);
    }
    return false; // because another action is called in sequence
}

bool GumpActions::selectServerFirst(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    components::BaseButton* button = dynamic_cast<components::BaseButton*>(menu->get_named_item("selectserver"));
    if (!button) {
        LOG_DEBUG << "No server found for quickselect" << std::endl;
    } else {
        doAction(button);
    }
    return false; // because another action is called in sequence
}

bool GumpActions::selectCharacterFirst(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    components::BaseButton* button = dynamic_cast<components::BaseButton*>(menu->get_named_item("selectcharacter"));
    if (!button) {
        LOG_DEBUG << "No character found for quickselect" << std::endl;
    } else {
        doAction(button);
    }
    return false; // because another action is called in sequence
}

bool GumpActions::sendSpeech(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    components::LineEdit* line = dynamic_cast<components::LineEdit*>(menu->get_named_item("speechtext"));
    if (line) {
        UnicodeString speechText = line->getText();
        if (speechText.length() > 0) {
            ui::Manager::getCommandManager()->handleTextInput(speechText);
            line->setText("");
            line->set_focus(false);
            menu->activatePage(0);
        }
    } else {
        LOG_ERROR << "Line element \"speechtext\" not found in gamewindow gump" << std::endl;
    }
    return true;
}

bool GumpActions::contextMenuReply(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    Serial serial = StringConverter::toInt(parameters[0]);
    unsigned int replyId = StringConverter::toInt(parameters[1]);
    boost::shared_ptr<net::Packet> subPacket(new net::packets::bf::ContextMenuReply(serial, replyId));
    net::packets::BF pkt(subPacket);
    net::Manager::getSingleton()->send(pkt);

    return true;
}

bool GumpActions::createDummyCharacter(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    net::packets::CreateCharacter pkt;
    
    pkt.name_ = "Dummy";
    pkt.slot_ = 0;
    
    
    net::Manager::getSingleton()->send(pkt);

    return true;
}

bool GumpActions::openStatus(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    world::Mobile* mob = menu->getLinkedMobile();
    if (mob) {
        mob->openStatWindow(ui::Manager::getSingleton()->getMousePosition());
    } else {
        LOG_WARN << "openstatus gump action in unlinked gump" << std::endl;
    }
    return true;
}

bool GumpActions::helpRequest(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    LOG_DEBUG << "send help request" << std::endl;
    net::packets::HelpRequest pkt;
    net::Manager::getSingleton()->send(pkt);

    return true;
}

}
}
