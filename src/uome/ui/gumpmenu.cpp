
#include "gumpmenu.hpp"

#include "manager.hpp"

namespace uome {
namespace ui {

GumpMenu::GumpMenu(const CL_GUITopLevelDescription& desc) :
    CL_Window(ui::Manager::getSingleton()->getGuiManager().get(), desc) {
    set_draggable(true);
}

}
}
