
#include "serverbutton.hpp"

namespace uome {
namespace ui {
namespace components {

ServerButton::ServerButton(CL_GUIComponent* parent, unsigned int buttonId) : BaseButton(parent), buttonId_(buttonId) {
}

unsigned int ServerButton::getButtonId() {
    return buttonId_;
}

void ServerButton::onClicked(BaseButton* self) {
}

}
}
}
