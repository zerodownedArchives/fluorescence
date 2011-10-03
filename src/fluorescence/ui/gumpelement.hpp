#ifndef FLUO_UI_GUMPELEMENT_HPP
#define FLUO_UI_GUMPELEMENT_HPP

#include <ClanLib/gui.h>

namespace fluo {
namespace ui {

class GumpMenu;

class GumpElement : public CL_GUIComponent {
public:
    GumpElement(CL_GUIComponent* parent);
};

}
}

#endif
