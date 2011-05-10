
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

#include "gumpmenu.hpp"
#include "gumpfactory.hpp"
#include "components/localbutton.hpp"
#include "components/lineedit.hpp"
#include "components/label.hpp"

namespace uome {
namespace ui {

GumpMenu* GumpMenus::openMessageBox(const UnicodeString& message) {
    GumpMenu* menu = GumpFactory::fromXmlFile("messagebox");
    if (menu) {
        menu->setComponentText<components::Label>("messagetext", message);
    }

    LOG_INFO << "MessageBox: " << message << std::endl;

    return menu;
}

GumpMenu* GumpMenus::openLoginGump() {
    GumpMenu* menu = GumpFactory::fromXmlFile("login");

    if (menu) {
        Config& config = Client::getSingleton()->getConfig();
        menu->setComponentTextFromConfig<components::LineEdit>("loginhost", "/uome/shard/address@host", config);

        if (config.exists("/uome/shard/address@port")) {
            UnicodeString portStr = StringConverter::fromNumber(config["/uome/shard/address@port"].asInt());

            menu->setComponentText<components::LineEdit>("loginport", portStr);
        }

        menu->setComponentTextFromConfig<components::LineEdit>("loginaccount", "/uome/shard/account@name", config);
        menu->setComponentTextFromConfig<components::LineEdit>("loginpassword", "/uome/shard/account@password", config);

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

    GumpFactory::RepeatContext context;
    context.repeatCount_ = nameList.size();
    context.keywordReplacments_[GumpFactory::RepeatKeyword("tbutton", "param", "shardname")] = nameList;
    context.keywordReplacments_[GumpFactory::RepeatKeyword("tbutton", "text", "shardname")] = nameList;

    GumpFactory::addRepeatContext("shardlist", context);
    GumpMenu* menu = GumpFactory::fromXmlFile("shardselection");
    GumpFactory::removeRepeatContext("shardlist");

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

    GumpFactory::RepeatContext context;
    context.repeatCount_ = nameList.size();
    context.keywordReplacments_[GumpFactory::RepeatKeyword("tbutton", "param", "serverindex")] = indexList;
    context.keywordReplacments_[GumpFactory::RepeatKeyword("tbutton", "text", "servername")] = nameList;

    GumpFactory::addRepeatContext("serverlist", context);
    GumpMenu* menu = GumpFactory::fromXmlFile("serverlist");
    GumpFactory::removeRepeatContext("serverlist");

    return menu;
}

GumpMenu* GumpMenus::openCharacterListGump(const net::packets::CharacterList* list) {
    std::vector<UnicodeString> nameList;
    std::vector<UnicodeString> indexList;

    for (unsigned int i = 0; i < list->charCount_; ++i) {
        nameList.push_back(list->charNames_[i]);
        indexList.push_back(StringConverter::fromNumber(i));
    }

    GumpFactory::RepeatContext context;
    context.repeatCount_ = nameList.size();
    context.keywordReplacments_[GumpFactory::RepeatKeyword("tbutton", "param", "characterindex")] = indexList;
    context.keywordReplacments_[GumpFactory::RepeatKeyword("tbutton", "text", "charactername")] = nameList;

    GumpFactory::addRepeatContext("characterlist", context);
    GumpMenu* menu = GumpFactory::fromXmlFile("characterlist");
    GumpFactory::removeRepeatContext("characterlist");

    return menu;
}

}
}
