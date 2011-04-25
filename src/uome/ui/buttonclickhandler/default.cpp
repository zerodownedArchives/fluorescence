
#include "default.hpp"

#include <misc/logger.hpp>

namespace uome {
namespace ui {
namespace buttonclickhandler {

void Default::onButtonClicked(ui::components::BaseButton* button) {
    LOGARG_INFO(LOGTYPE_UI, "Unhandled button click, id=%i", button->getButtonId());
}

}
}
}
