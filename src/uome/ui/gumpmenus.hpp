#ifndef UOME_UI_GUMPMENUS_HPP
#define UOME_UI_GUMPMENUS_HPP

#include <string>
#include <boost/shared_ptr.hpp>

namespace uome {

class Config;

namespace net {
namespace packets {
class ServerList;
}
}

namespace ui {

class GumpMenu;

class GumpMenus {
public:
    static GumpMenu* openMessageBox(const std::string& message);
    static GumpMenu* openShardSelectionGump();
    static GumpMenu* openLoginGump();
    static GumpMenu* openServerListGump(const net::packets::ServerList* list);
};

}
}

#endif
