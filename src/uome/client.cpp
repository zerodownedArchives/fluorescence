
#include "client.hpp"

#include "config.hpp"

#include <ui/manager.hpp>
#include <ui/ingamewindow.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/gumpartloader.hpp>
#include <data/maploader.hpp>
#include <data/staticsloader.hpp>
#include <data/maptexloader.hpp>

#include <world/sector.hpp>
#include <world/map.hpp>
#include <world/statics.hpp>

#include <iostream>
#include <iomanip>

namespace uome {

Client::Client() {
}

int Client::sMain(const std::vector<CL_String8>& args) {
    Client instance;
    return instance.main(args);
}

bool Client::checkSectorRemove(const boost::shared_ptr<uome::world::Sector>& ptr) {
    return false;//(ptr->getLocX() + ptr->getLocY()) % 3 == 0;
}

int Client::main(const std::vector<CL_String8>& args) {
    LOG_INFO(LOGTYPE_MAIN, "Starting client");

    LOG_INFO(LOGTYPE_MAIN, "Parsing configuration");
    if (!Config::getVariablesMap(args, config_)) {
        return 1;
    }

    LOG_INFO(LOGTYPE_MAIN, "Creating data loaders");
    if (!data::Manager::create(config_)) {
        LOG_DEBUG(LOGTYPE_MAIN, "Unable to intialize manager, exiting!");
        return 1;
    }

    LOG_INFO(LOGTYPE_MAIN, "Initializing ui");
    if (!ui::Manager::create(config_)) {
        return 1;
    }


    boost::shared_ptr<CL_DisplayWindow> wnd = uome::ui::Manager::getSingleton()->getWindow();
    CL_InputContext ic = uome::ui::Manager::getSingleton()->getIC();
    boost::shared_ptr<ui::IngameWindow> ingameWindow = uome::ui::Manager::getSingleton()->getIngameWindow();
    ingameWindow->setCenterTiles(172 * 8, 202 * 8);

    std::list<boost::shared_ptr<uome::world::Sector> > sectorList;


    timeval lastTime;
    gettimeofday(&lastTime, NULL);


    for (unsigned int i = 1; !ic.get_keyboard().get_keycode(CL_KEY_ESCAPE); ++i) {
        if (i == 1) {
            for (unsigned int l = 200; l < 204; ++l) {
                for (unsigned int m = 170; m < 180; ++m) {
                    boost::shared_ptr<uome::world::Sector> newSec(new uome::world::Sector(m * 512 + l, m, l));
                    sectorList.push_back(newSec);
                }
            }
        }

        if (i == 1000) { // ~ 7 sec
            for (unsigned int l = 204; l < 207; ++l) {
                for (unsigned int m = 173; m < 177; ++m) {
                    boost::shared_ptr<uome::world::Sector> newSec(new uome::world::Sector(m * 512 + l, m, l));
                    sectorList.push_back(newSec);
                }
            }
        }

        if (i == 1500) { // ~ 7 sec
            for (unsigned int l = 207; l < 210; ++l) {
                for (unsigned int m = 170; m < 180; ++m) {
                    boost::shared_ptr<uome::world::Sector> newSec(new uome::world::Sector(m * 512 + l, m, l));
                    sectorList.push_back(newSec);
                }
            }
        }

        if (i % 50 == 0) {
            timeval curTime;
            gettimeofday(&curTime, NULL);

            float diff = (curTime.tv_sec - lastTime.tv_sec);
            diff += (curTime.tv_usec - lastTime.tv_usec) / 1000000.0f;

            float fps = diff / 50.0f;
            fps = 1/fps;

            std::ostringstream titleHelper;
            titleHelper << "UO:ME -- fps: " << std::setiosflags(std::ios::fixed) << std::setprecision(1) << fps;
            wnd->set_title(titleHelper.str());
            LOGARG_DEBUG(LOGTYPE_MAIN, "fps: %.1f", fps);
             lastTime = curTime;
        }

        if (ic.get_keyboard().get_keycode(CL_KEY_DOWN)) {
            ingameWindow->setCenterTiles(ingameWindow->getCenterTileX(), ingameWindow->getCenterTileY() + 1);
        } else if (ic.get_keyboard().get_keycode(CL_KEY_UP)) {
            ingameWindow->setCenterTiles(ingameWindow->getCenterTileX(), ingameWindow->getCenterTileY() - 1);
        }

        if (ic.get_keyboard().get_keycode(CL_KEY_LEFT)) {
            ingameWindow->setCenterTiles(ingameWindow->getCenterTileX() - 1, ingameWindow->getCenterTileY());
        } else if (ic.get_keyboard().get_keycode(CL_KEY_RIGHT)) {
            ingameWindow->setCenterTiles(ingameWindow->getCenterTileX() + 1, ingameWindow->getCenterTileY());
        }


        // remove sectors no longer used
        if (i == 2000) {
            sectorList.remove_if(checkSectorRemove);
        }

        // call renderer
        ingameWindow->renderOneFrame();
        wnd->flip();

        CL_KeepAlive::process();
        //CL_System::sleep(10);
    }

    return 0;
}

}
