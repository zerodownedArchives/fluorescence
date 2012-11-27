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
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#include <client.hpp>

#include <ui/manager.hpp>
#include <ui/commandmanager.hpp>
#include <ui/components/button.hpp>
#include <ui/components/lineedit.hpp>
#include <ui/components/checkbox.hpp>

#include <net/manager.hpp>
#include <net/packets/00_createcharacter.hpp>
#include <net/packets/ad_speechrequest.hpp>
#include <net/packets/bf.hpp>
#include <net/packets/bf/15_contextmenureply.hpp>

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
    actionTable_["close"] = GumpAction(true, boost::bind(&GumpActions::closeHelper, _1, _2, _3, _4));
}

void GumpActions::buildFullActionTable() {
    actionTable_["deletecharacter"] = GumpAction(true, boost::bind(&Client::deleteCharacter, Client::getSingleton(), _1, _2, _3, _4));

    actionTable_["createdummychar"] = GumpAction(true, boost::bind(&GumpActions::createDummyCharacter, _1, _2, _3, _4));

    actionTable_["yesnogump"] = GumpAction(false, boost::bind(&GumpActions::yesNoGump, _1, _2, _3, _4));
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

void GumpActions::doAction(components::Button* button) {
    const GumpMenu* gump = button->getGumpMenu();
    if (gump) {
        //doAction(gump, button->getAction(), components::Button::MAX_PARAMETER_COUNT, button->getParameterPtr());
    } else {
        LOG_ERROR << "Button inside something other than GumpMenu" << std::endl;
    }
}


bool GumpActions::closeHelper(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    return true;
}

bool GumpActions::createDummyCharacter(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    net::packets::CreateCharacter pkt;

    pkt.name_ = "Dummy";
    pkt.slot_ = 0;
    pkt.startCity_ = 1;

    net::Manager::getSingleton()->send(pkt);

    return true;
}

bool GumpActions::yesNoGump(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    GumpMenus::openYesNoBox(action, parameterCount, parameters);
    return true;
}

}
}
