
#include "localbutton.hpp"

#include <boost/bind.hpp>

#include <client.hpp>
#include <ui/manager.hpp>
#include <net/manager.hpp>

#include <misc/logger.hpp>

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
}

LocalButton::LocalButton(CL_GUIComponent* parent, const UnicodeString& action, const UnicodeString& parameter) : BaseButton(parent),
        action_(action), parameter_(parameter) {
}

void LocalButton::setAction(const UnicodeString& value) {
    action_ = value;
}

const UnicodeString& LocalButton::getAction() const {
    return action_;
}

void LocalButton::setParameter(const UnicodeString& value) {
    parameter_ = value;
}

const UnicodeString& LocalButton::getParameter() const {
    return parameter_;
}

int LocalButton::getParameterInt() const {
    std::string utfStr = StringConverter::toUtf8String(parameter_);
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
