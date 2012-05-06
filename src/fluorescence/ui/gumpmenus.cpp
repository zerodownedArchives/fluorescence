/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "gumpmenus.hpp"

#include <sstream>
#include <list>
#include <vector>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <client.hpp>
#include <misc/log.hpp>
#include <misc/config.hpp>
#include <misc/exception.hpp>

#include <net/packets/serverlist.hpp>
#include <net/packets/characterlist.hpp>
#include <net/packets/bf/opencontextmenu.hpp>

#include <data/manager.hpp>
#include <data/clilocloader.hpp>

#include "gumpmenu.hpp"
#include "xmlparser.hpp"
#include "components/lineedit.hpp"
#include "components/label.hpp"

namespace fluo {
namespace ui {

GumpMenu* GumpMenus::openMessageBox(const UnicodeString& message) {
    GumpMenu* menu = XmlParser::fromXmlFile("messagebox");
    if (menu) {
        menu->setComponentText<components::Label>("messagetext", message);
    }

    LOG_INFO << "MessageBox: " << message << std::endl;

    return menu;
}

GumpMenu* GumpMenus::openLoginGump() {
    GumpMenu* menu = XmlParser::fromXmlFile("login");

    if (menu) {
        Config& config = Client::getSingleton()->getConfig();
        menu->setComponentTextFromConfig<components::LineEdit>("loginhost", "/fluo/shard/address@host", config);

        if (config.exists("/fluo/shard/address@port")) {
            UnicodeString portStr = StringConverter::fromNumber(config["/fluo/shard/address@port"].asInt());

            menu->setComponentText<components::LineEdit>("loginport", portStr);
        }

        menu->setComponentTextFromConfig<components::LineEdit>("loginaccount", "/fluo/shard/account@name", config);
        menu->setComponentTextFromConfig<components::LineEdit>("loginpassword", "/fluo/shard/account@password", config);

    }

    return menu;
}

GumpMenu* GumpMenus::openShardSelectionGump() {
    namespace bfs = boost::filesystem;

    bfs::path path("shards");

    if (!bfs::exists(path) || !bfs::is_directory(path)) {
        LOG_EMERGENCY << "Unable to list shards directory" << std::endl;
        return NULL;
    }

    std::vector<UnicodeString> nameList;

    bfs::directory_iterator nameIter(path);
    bfs::directory_iterator nameEnd;

    for (; nameIter != nameEnd; ++nameIter) {
        if (bfs::is_directory(nameIter->status())) {
            nameList.push_back(StringConverter::fromUtf8(nameIter->path().filename()));
        }
    }

    XmlParser::RepeatContext context;
    context.repeatCount_ = nameList.size();
    context.keywordReplacments_[XmlParser::RepeatKeyword("tbutton", "param", "shardname")] = nameList;
    context.keywordReplacments_[XmlParser::RepeatKeyword("tbutton", "text", "shardname")] = nameList;

    XmlParser::addRepeatContext("shardlist", context);
    GumpMenu* menu = XmlParser::fromXmlFile("shardselection");
    XmlParser::removeRepeatContext("shardlist");

    return menu;
}

GumpMenu* GumpMenus::openServerListGump(const net::packets::ServerList* list) {
    std::vector<UnicodeString> nameList;
    std::vector<UnicodeString> indexList;

    std::list<net::packets::ServerList::ServerListEntry>::const_iterator entryIter = list->listEntries_.begin();
    std::list<net::packets::ServerList::ServerListEntry>::const_iterator entryEnd = list->listEntries_.end();


    for (; entryIter != entryEnd; ++entryIter) {
        nameList.push_back(entryIter->name_);

        indexList.push_back(StringConverter::fromNumber(entryIter->index_));
    }

    XmlParser::RepeatContext context;
    context.repeatCount_ = nameList.size();
    context.keywordReplacments_[XmlParser::RepeatKeyword("tbutton", "param", "serverindex")] = indexList;
    context.keywordReplacments_[XmlParser::RepeatKeyword("tbutton", "text", "servername")] = nameList;

    XmlParser::addRepeatContext("serverlist", context);
    GumpMenu* menu = XmlParser::fromXmlFile("serverlist");
    XmlParser::removeRepeatContext("serverlist");

    return menu;
}

GumpMenu* GumpMenus::openCharacterListGump(const net::packets::CharacterList* list) {
    std::vector<UnicodeString> nameList;
    std::vector<UnicodeString> indexList;
    std::vector<UnicodeString> passwordList;

    for (unsigned int i = 0; i < list->charCount_; ++i) {
        nameList.push_back(list->charNames_[i]);
        indexList.push_back(StringConverter::fromNumber(i));
        passwordList.push_back(list->charPasswords_[i]);
    }

    XmlParser::RepeatContext context;
    context.repeatCount_ = nameList.size();
    context.keywordReplacments_[XmlParser::RepeatKeyword("tbutton", "param", "characterindex")] = indexList;
    context.keywordReplacments_[XmlParser::RepeatKeyword("tbutton", "text", "charactername")] = nameList;
    context.keywordReplacments_[XmlParser::RepeatKeyword("tbutton", "param2", "characterpassword")] = nameList;

    XmlParser::addRepeatContext("characterlist", context);
    GumpMenu* menu = XmlParser::fromXmlFile("characterlist");
    XmlParser::removeRepeatContext("characterlist");

    return menu;
}

GumpMenu* GumpMenus::openContextMenu(const net::packets::bf::OpenContextMenu* pkt) {
    std::vector<UnicodeString> nameList;
    std::vector<UnicodeString> indexList;
    std::vector<UnicodeString> serialList;

    std::list<net::packets::bf::ContextMenuEntry>::const_iterator iter = pkt->entries_.begin();
    std::list<net::packets::bf::ContextMenuEntry>::const_iterator end = pkt->entries_.end();

    UnicodeString serialString = StringConverter::fromNumber(pkt->serial_);
    for (; iter != end; ++iter) {
        indexList.push_back(StringConverter::fromNumber(iter->entryId_));
        nameList.push_back(data::Manager::getClilocLoader()->get(iter->clilocId_));
        serialList.push_back(serialString);
    }

    XmlParser::RepeatContext context;
    context.repeatCount_ = nameList.size();
    context.keywordReplacments_[XmlParser::RepeatKeyword("tbutton", "text", "entrytext")] = nameList;
    context.keywordReplacments_[XmlParser::RepeatKeyword("tbutton", "param", "serial")] = serialList;
    context.keywordReplacments_[XmlParser::RepeatKeyword("tbutton", "param2", "entryindex")] = indexList;

    XmlParser::addRepeatContext("contextmenu", context);
    GumpMenu* menu = XmlParser::fromXmlFile("contextmenu");
    XmlParser::removeRepeatContext("contextmenu");

    return menu;
}

GumpMenu* GumpMenus::openSpellbook(const world::DynamicItem* itm) {
    GumpMenu* menu = XmlParser::fromXmlFile("spellbook");
    
    return menu;
}

}
}
