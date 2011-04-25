#ifndef UOME_UI_BUTTONCLICKHANDLER_DEFAULT_HPP
#define UOME_UI_BUTTONCLICKHANDLER_DEFAULT_HPP

#include <ui/buttonclickhandler.hpp>
#include <ui/components/basebutton.hpp>

namespace uome {
namespace ui {
namespace buttonclickhandler {

class Default : public ButtonClickHandler {
public:
    virtual void onButtonClicked(ui::components::BaseButton* button);
};

}
}
}

#endif
