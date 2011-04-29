
#include "gumpmenus.hpp"

#include <sstream>

#include <client.hpp>
#include <misc/logger.hpp>
#include <misc/config.hpp>

#include "gumpmenu.hpp"
#include "gumpfactory.hpp"

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

}
}
