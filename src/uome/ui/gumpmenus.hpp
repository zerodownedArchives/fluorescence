#ifndef UOME_UI_GUMPMENUS_HPP
#define UOME_UI_GUMPMENUS_HPP

#include <string>


namespace uome {

class Config;

namespace ui {

class GumpMenu;

class GumpMenus {
public:
    static GumpMenu* openMessageBox(const std::string& message);
    static GumpMenu* openShardSelectionGump();
    static GumpMenu* openLoginGump();
};

}
}

#endif
