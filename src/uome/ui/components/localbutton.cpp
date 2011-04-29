
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

LocalButton::ClickAction::ClickAction(bool closeGumpMenu, boost::function<bool (GumpMenu*, const std::string&)> callback) : closeGumpMenu_(closeGumpMenu), callback_(callback) {
}

std::map<std::string, LocalButton::ClickAction> LocalButton::actionTable_ = std::map<std::string, LocalButton::ClickAction>();

void LocalButton::buildBasicActionTable() {
    actionTable_["shutdown"] = ClickAction(false, boost::bind(&Client::shutdown, Client::getSingleton(), _1, _2));
    actionTable_["selectshard"] = ClickAction(true, boost::bind(&Client::selectShard, Client::getSingleton(), _1, _2));
}

void LocalButton::buildFullActionTable() {
    actionTable_["connect"] = ClickAction(false, boost::bind(&net::Manager::connect, net::Manager::getSingleton(), _1, _2));
}

LocalButton::LocalButton(CL_GUIComponent* parent, const std::string& action, const std::string& parameter) : BaseButton(parent),
        action_(action), parameter_(parameter) {
}

const std::string& LocalButton::getAction() {
    return action_;
}

const std::string& LocalButton::getParameter() {
    return parameter_;
}

void LocalButton::onClicked(BaseButton* self) {
    GumpMenu* gump = dynamic_cast<GumpMenu*>(get_top_level_component());
    if (gump) {
        std::map<std::string, ClickAction>::iterator it = actionTable_.find(action_);

        if (it != actionTable_.end()) {
            bool funcRet = (it->second.callback_) (gump, parameter_);
            if (funcRet && it->second.closeGumpMenu_) { // close gump if callback was successful
                ui::Manager::getSingleton()->closeGumpMenu(gump);
            }
        } else {
            LOGARG_ERROR(LOGTYPE_UI, "Unknown button action: %s", action_.c_str());
        }
    } else {
        LOG_ERROR(LOGTYPE_UI, "PageButton inside something other than GumpMenu");
    }
}

}
}
}
