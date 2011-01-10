
#include "gumpmenu.hpp"

#include "manager.hpp"

namespace uome {
namespace ui {

GumpMenu::GumpMenu(const CL_Rect& bounds) :
    CL_Window(
        ui::Manager::getSingleton()->getGuiManager().get(),
        CL_GUITopLevelDescription(bounds, false)
    ) {
}

}
}
