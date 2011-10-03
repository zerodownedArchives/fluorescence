
#include "serverbutton.hpp"

#include <ui/manager.hpp>
#include <ui/gumpmenu.hpp>

namespace fluo {
namespace ui {
namespace components {

ServerButton::ServerButton(CL_GUIComponent* parent, unsigned int buttonId) : BaseButton(parent), buttonId_(buttonId) {
}

unsigned int ServerButton::getButtonId() {
    return buttonId_;
}

void ServerButton::onClicked(BaseButton* self) {
    GumpMenu* gump = dynamic_cast<GumpMenu*>(get_top_level_component());
    if (gump) {
        if (buttonId_ == 0) {
            ui::Manager::getSingleton()->closeGumpMenu(gump);
        } else {
            // TODO: send gump reply to server
        }
    }
}

}
}
}
