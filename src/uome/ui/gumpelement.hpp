#ifndef UOME_UI_GUMPELEMENT_HPP
#define UOME_UI_GUMPELEMENT_HPP

#include <ClanLib/gui.h>

namespace uome {
namespace ui {

class GumpMenu;

class GumpElement : public CL_GUIComponent {
public:
    GumpElement(GumpMenu* parent);
};

}
}

#endif
