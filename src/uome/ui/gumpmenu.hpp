#ifndef UOME_UI_GUMPMENU_HPP
#define UOME_UI_GUMPMENU_HPP

#include <ClanLib/gui.h>

#include <boost/enable_shared_from_this.hpp>

namespace uome {
namespace ui {

class GumpMenu : public CL_Window, public boost::enable_shared_from_this<GumpMenu> {
public:
    GumpMenu(const CL_GUITopLevelDescription& desc);
};

}
}

#endif
