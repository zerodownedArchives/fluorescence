#ifndef UOME_UI_COMPONENTS_BASEBUTTON_HPP
#define UOME_UI_COMPONENTS_BASEBUTTON_HPP

#include <ClanLib/GUI/Components/push_button.h>

namespace uome {
namespace ui {
namespace components {

class BaseButton : public CL_PushButton {
public:
    BaseButton(CL_GUIComponent* parent);

    virtual void onClicked(BaseButton* self) = 0;
};

}
}
}

#endif
