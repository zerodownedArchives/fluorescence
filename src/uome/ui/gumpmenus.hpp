#ifndef UOME_UI_GUMPMENUS_HPP
#define UOME_UI_GUMPMENUS_HPP

#include <misc/string.hpp>

namespace uome {

class Config;

namespace net {
namespace packets {
class ServerList;
class CharacterList;
}
}

namespace ui {

class GumpMenu;

class GumpMenus {
public:
    static GumpMenu* openMessageBox(const UnicodeString& message);
    static GumpMenu* openShardSelectionGump();
    static GumpMenu* openLoginGump();
    static GumpMenu* openServerListGump(const net::packets::ServerList* list);
    static GumpMenu* openCharacterListGump(const net::packets::CharacterList* list);
};

}
}

#endif
