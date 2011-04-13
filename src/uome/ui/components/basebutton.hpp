#ifndef UOME_UI_COMPONENTS_BASEBUTTON_HPP
#define UOME_UI_COMPONENTS_BASEBUTTON_HPP

#include <ClanLib/GUI/Components/push_button.h>

namespace uome {
namespace ui {
namespace components {

class BaseButton : public CL_PushButton {
public:
    BaseButton(CL_GUIComponent* parent);

    void setButtonId(unsigned int value);
    unsigned int getButtonId();

    void setPageId(unsigned int value);
    unsigned int getPageId();

private:
    unsigned int buttonId_;
    unsigned int pageId_;
};

}
}
}

#endif
