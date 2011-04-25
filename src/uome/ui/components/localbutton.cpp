
#include "localbutton.hpp"

#include <client.hpp>
#include <misc/logger.hpp>

namespace uome {
namespace ui {
namespace components {

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

    if (action_ == "selectshard") {
        Client::getSingleton()->getConfig()->set("shard", parameter_);
        Client::getSingleton()->setState(Client::STATE_LOGIN);
    } else if (action_ == "shutdown") {
        Client::getSingleton()->shutdown();
    } else {
        LOGARG_ERROR(LOGTYPE_UI, "Unknown button action: %s", action_.c_str());
    }
}

}
}
}
