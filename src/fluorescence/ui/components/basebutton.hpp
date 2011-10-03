#ifndef FLUO_UI_COMPONENTS_BASEBUTTON_HPP
#define FLUO_UI_COMPONENTS_BASEBUTTON_HPP

#include <ClanLib/GUI/Components/push_button.h>

#include <misc/string.hpp>

namespace fluo {
namespace ui {
namespace components {

class BaseButton : public CL_PushButton {
public:
    BaseButton(CL_GUIComponent* parent);

    UnicodeString getText();
    void setText(const UnicodeString& string);

    virtual void onClicked(BaseButton* self) = 0;
};

}
}
}

#endif
