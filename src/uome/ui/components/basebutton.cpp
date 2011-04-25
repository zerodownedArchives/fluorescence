
#include "basebutton.hpp"

namespace uome {
namespace ui {
namespace components {

BaseButton::BaseButton(CL_GUIComponent* parent) : CL_PushButton(parent) {
    func_clicked().set(this, &BaseButton::onClicked, this);
}

}
}
}
