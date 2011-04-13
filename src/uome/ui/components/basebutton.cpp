
#include "basebutton.hpp"

namespace uome {
namespace ui {
namespace components {

BaseButton::BaseButton(CL_GUIComponent* parent) : CL_PushButton(parent), buttonId_(0) {
}

void BaseButton::setButtonId(unsigned int value) {
    buttonId_ = value;
}

unsigned int BaseButton::getButtonId() {
    return buttonId_;
}

void BaseButton::setPageId(unsigned int value) {
    pageId_ = value;
}

unsigned int BaseButton::getPageId() {
    return pageId_;
}


}
}
}
