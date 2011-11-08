#ifndef FLUO_UI_GUMPMENUS_HPP
#define FLUO_UI_GUMPMENUS_HPP

#include <misc/string.hpp>

namespace fluo {

class Config;

namespace net {
namespace packets {
class ServerList;
class CharacterList;

namespace bf {
class OpenContextMenu;
}

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
    static GumpMenu* openContextMenu(const net::packets::bf::OpenContextMenu* pkt);
};

}
}

#endif
