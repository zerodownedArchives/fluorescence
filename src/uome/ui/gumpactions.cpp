
#include "gumpactions.hpp"

#include <boost/bind.hpp>

#include <client.hpp>
#include <ui/manager.hpp>
#include <net/manager.hpp>

#include <misc/log.hpp>

namespace uome {
namespace ui {

GumpActions::GumpAction::GumpAction() {
}

GumpActions::GumpAction::GumpAction(bool closeGumpMenu, GumpActionCallback callback) : closeGumpMenu_(closeGumpMenu), callback_(callback) {
}

std::map<UnicodeString, GumpActions::GumpAction> GumpActions::actionTable_ = std::map<UnicodeString, GumpActions::GumpAction>();

void GumpActions::buildBasicActionTable() {
    actionTable_["shutdown"] = GumpAction(false, boost::bind(&Client::shutdown, Client::getSingleton(), _1, _2, _3, _4));
    actionTable_["selectshard"] = GumpAction(true, boost::bind(&Client::selectShard, Client::getSingleton(), _1, _2, _3, _4));
    actionTable_["close"] = GumpAction(true, boost::bind(&GumpActions::closeHelper, _1, _2, _3, _4));
}

void GumpActions::buildFullActionTable() {
    actionTable_["connect"] = GumpAction(false, boost::bind(&net::Manager::connect, net::Manager::getSingleton(), _1, _2, _3, _4));
    actionTable_["disconnect"] = GumpAction(true, boost::bind(&Client::disconnect, Client::getSingleton(), _1, _2, _3, _4));
    actionTable_["selectserver"] = GumpAction(false, boost::bind(&net::Manager::selectServer, net::Manager::getSingleton(), _1, _2, _3, _4));
    actionTable_["selectcharacter"] = GumpAction(true, boost::bind(&Client::selectCharacter, Client::getSingleton(), _1, _2, _3, _4));
}


bool GumpActions::closeHelper(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    return true;
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

}
}