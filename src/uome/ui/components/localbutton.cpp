
#include "localbutton.hpp"

#include <misc/log.hpp>

#include <ui/gumpactions.hpp>

namespace uome {
namespace ui {
namespace components {

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

const UnicodeString* LocalButton::getParameterPtr() const {
    return parameter_;
}

void LocalButton::onClicked(BaseButton* self) {
    GumpActions::doAction(this);
}

}
}
}
