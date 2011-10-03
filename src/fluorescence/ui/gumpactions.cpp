
#include "gumpactions.hpp"

#include <boost/bind.hpp>

#include <client.hpp>

#include <ui/manager.hpp>
#include <ui/components/lineedit.hpp>

#include <net/manager.hpp>
#include <net/packets/speechrequest.hpp>

#include <misc/log.hpp>


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

void GumpActions::doAction(components::LocalButton* button) {
    GumpMenu* gump = dynamic_cast<GumpMenu*>(button->get_top_level_component());
    if (gump) {
        doAction(gump, button->getAction(), MAX_PARAMETER_COUNT, button->getParameterPtr());
    } else {
        LOG_ERROR << "LocalButton inside something other than GumpMenu" << std::endl;
    }
}


bool GumpActions::closeHelper(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    return true;
}

bool GumpActions::selectShardFirst(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    components::LocalButton* button = dynamic_cast<components::LocalButton*>(menu->get_named_item("selectshard"));
    if (!button) {
        doAction(menu, "createshard", 0, NULL);
    } else {
        doAction(button);
    }

    return false; // because another action is called in sequence
}

bool GumpActions::selectServerFirst(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    components::LocalButton* button = dynamic_cast<components::LocalButton*>(menu->get_named_item("selectserver"));
    if (!button) {
        doAction(menu, "disconnect", 0, NULL);
    } else {
        doAction(button);
    }
    return false; // because another action is called in sequence
}

bool GumpActions::selectCharacterFirst(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    components::LocalButton* button = dynamic_cast<components::LocalButton*>(menu->get_named_item("selectcharacter"));
    if (!button) {
        doAction(menu, "disconnect", 0, NULL);
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
            net::packets::SpeechRequest pkt(speechText);
            net::Manager::getSingleton()->send(pkt);
            line->setText("");
            line->set_focus(false);
            menu->activatePage(0);
        }
    } else {
        LOG_ERROR << "Line element \"speechtext\" not found in gamewindow gump" << std::endl;
    }
    return true;
}

}
}
