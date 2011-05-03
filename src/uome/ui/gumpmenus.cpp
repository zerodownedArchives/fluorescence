
#include "gumpmenus.hpp"

#include <sstream>
#include <list>
#include <vector>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <client.hpp>
#include <misc/logger.hpp>
#include <misc/config.hpp>

#include <net/packets/serverlist.hpp>

#include "gumpmenu.hpp"
#include "gumpfactory.hpp"
#include "components/localbutton.hpp"

namespace uome {
namespace ui {

GumpMenu* GumpMenus::openMessageBox(const std::string& message) {
    GumpMenu* menu = GumpFactory::fromXmlFile("messagebox");
    if (menu) {
        menu->setComponentText<CL_Label>("messagetext", message);
    }

    LOGARG_INFO(LOGTYPE_UI, "MessageBox: %s", message.c_str());

    return menu;
}

GumpMenu* GumpMenus::openLoginGump() {
    GumpMenu* menu = GumpFactory::fromXmlFile("login");

    if (menu) {
        Config* config = Client::getSingleton()->getConfig();
        menu->setComponentTextFromConfig<CL_LineEdit>("loginhost", "shard.host", config);

        if (config->count("shard.port") > 0) {
            std::stringstream ss;
            ss << config->get("shard.port").as<unsigned short>();
            menu->setComponentText<CL_LineEdit>("loginport", ss.str());
        }

        menu->setComponentTextFromConfig<CL_LineEdit>("loginaccount", "shard.account", config);
        menu->setComponentTextFromConfig<CL_LineEdit>("loginpassword", "shard.password", config);

    }

    return menu;
}

GumpMenu* GumpMenus::openShardSelectionGump() {
    namespace bfs = boost::filesystem;

    bfs::path path("shards");

    if (!bfs::exists(path) || !bfs::is_directory(path)) {
        LOG_ERROR(LOGTYPE_UI, "Unable to list shards directory");
        return NULL;
    }

    std::vector<std::string> nameList;

    bfs::directory_iterator nameIter(path);
    bfs::directory_iterator nameEnd;

    for (; nameIter != nameEnd; ++nameIter) {
        if (bfs::is_directory(nameIter->status())) {
            nameList.push_back(nameIter->path().filename());
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
    std::vector<std::string> nameList;
    std::vector<std::string> indexList;

    std::list<net::packets::ServerList::ServerListEntry>::const_iterator entryIter = list->listEntries_.begin();
    std::list<net::packets::ServerList::ServerListEntry>::const_iterator entryEnd = list->listEntries_.end();


    for (; entryIter != entryEnd; ++entryIter) {
        std::string serverName;
        serverName = entryIter->name_.toUTF8String(serverName);
        nameList.push_back(serverName);

        std::stringstream ss;
        ss << entryIter->index_;
        indexList.push_back(ss.str());
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

}
}
