#ifndef FLUO_UI_COMPONENTS_SERVERBUTTON_HPP
#define FLUO_UI_COMPONENTS_SERVERBUTTON_HPP

#include "basebutton.hpp"

namespace fluo {
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
