
#include "client.hpp"

#include <misc/config.hpp>
#include <misc/logger.hpp>

#include <ui/manager.hpp>
#include <ui/renderqueue.hpp>
#include <ui/ingamewindow.hpp>

#include <data/manager.hpp>

#include <world/manager.hpp>
#include <world/sectormanager.hpp>
#include <world/sector.hpp>

#include <iostream>
#include <iomanip>

namespace uome {

Client::Client() {
}

int Client::sMain(const std::vector<CL_String8>& args) {
    Client instance;
    return instance.main(args);
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

    LOG_INFO(LOGTYPE_MAIN, "Initializing world");
    if (!world::Manager::create(config_)) {
        return 1;
    }


    CL_DisplayWindow* wnd = uome::ui::Manager::getSingleton()->getWindow();
    CL_InputContext ic = uome::ui::Manager::getSingleton()->getIC();
    ui::IngameWindow* ingameWindow = uome::ui::Manager::getSingleton()->getIngameWindow();
    ingameWindow->setCenterTiles(172 * 8, 202 * 8);

    timeval lastTime;
    gettimeofday(&lastTime, NULL);


    for (unsigned int i = 1; !ic.get_keyboard().get_keycode(CL_KEY_ESCAPE); ++i) {
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
            //LOGARG_DEBUG(LOGTYPE_MAIN, "fps: %.1f", fps);
            lastTime = curTime;
        }

        if (i % 1 == 0) {

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

        }

        // adding sectors has to be done before sorting
        world::Manager::getSingleton()->getSectorManager()->addNewSectors();

        uome::ui::Manager::getSingleton()->getRenderQueue()->prepareRender();

        // deleting sectors has to be done after sorting (speed)
        world::Manager::getSingleton()->getSectorManager()->deleteSectors();

        // call renderer
        ingameWindow->renderOneFrame();
        wnd->flip();

        CL_KeepAlive::process();
        //CL_System::sleep(10);
    }

    // clean up
    world::Manager::destroy();
    ui::Manager::destroy();
    data::Manager::destroy();

    printf("end of main\n");

    return 0;
}

}
