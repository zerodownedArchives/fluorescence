#ifndef UOME_UI_BUTTONCLICKHANDLER_HPP
#define UOME_UI_BUTTONCLICKHANDLER_HPP

#include "components/basebutton.hpp"

namespace uome {
namespace ui {

class ButtonClickHandler {
public:
    virtual void onButtonClicked(components::BaseButton* button) = 0;
};

}
}

#endif
