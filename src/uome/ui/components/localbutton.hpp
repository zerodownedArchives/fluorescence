#ifndef UOME_UI_COMPONENTS_LOCALBUTTON_HPP
#define UOME_UI_COMPONENTS_LOCALBUTTON_HPP

#include "basebutton.hpp"

namespace uome {
namespace ui {
namespace components {

class LocalButton : public BaseButton {
public:
    LocalButton(CL_GUIComponent* parent, const std::string& action, const std::string& parameter = "");

    const std::string& getAction();
    const std::string& getParameter();

    virtual void onClicked(BaseButton* self);

private:
    std::string action_;
    std::string parameter_;
};

}
}
}

#endif
