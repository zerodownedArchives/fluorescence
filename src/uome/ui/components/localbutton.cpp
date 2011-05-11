
#include "localbutton.hpp"

#include <boost/bind.hpp>

#include <client.hpp>
#include <ui/manager.hpp>
#include <net/manager.hpp>

#include <misc/log.hpp>

namespace uome {
namespace ui {
namespace components {

LocalButton::ClickAction::ClickAction() {
}

LocalButton::ClickAction::ClickAction(bool closeGumpMenu, boost::function<bool (GumpMenu*, LocalButton*)> callback) : closeGumpMenu_(closeGumpMenu), callback_(callback) {
}

std::map<UnicodeString, LocalButton::ClickAction> LocalButton::actionTable_ = std::map<UnicodeString, LocalButton::ClickAction>();

void LocalButton::buildBasicActionTable() {
    actionTable_["shutdown"] = ClickAction(false, boost::bind(&Client::shutdown, Client::getSingleton(), _1, _2));
    actionTable_["selectshard"] = ClickAction(true, boost::bind(&Client::selectShard, Client::getSingleton(), _1, _2));
    actionTable_["close"] = ClickAction(true, boost::bind(&LocalButton::closeHelper, _1, _2));
}

void LocalButton::buildFullActionTable() {
    actionTable_["connect"] = ClickAction(false, boost::bind(&net::Manager::connect, net::Manager::getSingleton(), _1, _2));
    actionTable_["disconnect"] = ClickAction(true, boost::bind(&Client::disconnect, Client::getSingleton(), _1, _2));
    actionTable_["selectserver"] = ClickAction(false, boost::bind(&net::Manager::selectServer, net::Manager::getSingleton(), _1, _2));
    actionTable_["selectcharacter"] = ClickAction(true, boost::bind(&Client::selectCharacter, Client::getSingleton(), _1, _2));
}

LocalButton::LocalButton(CL_GUIComponent* parent, const UnicodeString& action) : BaseButton(parent),
        action_(action) {
}

void LocalButton::setAction(const UnicodeString& value) {
    action_ = value;
}

const UnicodeString& LocalButton::getAction() const {
    return action_;
}

void LocalButton::setParameter(const UnicodeString& value, unsigned int index) {
    if (index >= MAX_PARAMETER_COUNT) {
        LOG_INFO << "LocalButton::setParameter index too high: " << index << std::endl;
        index = 0;
    }
    parameter_[index] = value;
}

const UnicodeString& LocalButton::getParameter(unsigned int index) const {
    if (index >= MAX_PARAMETER_COUNT) {
        LOG_INFO << "LocalButton::getParameter index too high: " << index << std::endl;
        index = 0;
    }
    return parameter_[index];
}

int LocalButton::getParameterInt(unsigned int index) const {
    if (index >= MAX_PARAMETER_COUNT) {
        LOG_INFO << "LocalButton::getParameterInt index too high: " << index << std::endl;
        index = 0;
    }
    std::string utfStr = StringConverter::toUtf8String(parameter_[index]);
    return atoi(utfStr.c_str());
}

bool LocalButton::closeHelper(GumpMenu* menu, LocalButton* self) {
    return true;
}


void LocalButton::onClicked(BaseButton* self) {
    GumpMenu* gump = dynamic_cast<GumpMenu*>(get_top_level_component());
    if (gump) {
        std::map<UnicodeString, ClickAction>::iterator it = actionTable_.find(action_);

        if (it != actionTable_.end()) {
            bool funcRet = (it->second.callback_) (gump, this);
            if (funcRet && it->second.closeGumpMenu_) { // close gump if callback was successful
                ui::Manager::getSingleton()->closeGumpMenu(gump);
            }
        } else {
            LOG_ERROR << "Unknown button action: " << action_ << std::endl;
        }
    } else {
        LOG_ERROR << "PageButton inside something other than GumpMenu" << std::endl;
    }
}

}
}
}
