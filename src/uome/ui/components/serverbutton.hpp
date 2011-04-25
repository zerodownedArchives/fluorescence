#ifndef UOME_UI_COMPONENTS_SERVERBUTTON_HPP
#define UOME_UI_COMPONENTS_SERVERBUTTON_HPP

#include "basebutton.hpp"

namespace uome {
namespace ui {
namespace components {

class ServerButton : public BaseButton {
public:
    ServerButton(CL_GUIComponent* parent, unsigned int buttonId);

    unsigned int getButtonId();

    virtual void onClicked(BaseButton* self);

private:
    unsigned int buttonId_;
};

}
}
}

#endif
