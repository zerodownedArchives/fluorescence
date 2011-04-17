
#include "client.hpp"

#include <misc/config.hpp>
#include <misc/logger.hpp>

#include <ui/manager.hpp>
#include <ui/renderqueue.hpp>
#include <ui/ingameview.hpp>
#include <ui/gumpmenu.hpp>
#include <ui/gumpfactory.hpp>

#include <data/manager.hpp>

#include <world/manager.hpp>
#include <world/sectormanager.hpp>
#include <world/sector.hpp>

#include <iostream>
#include <iomanip>


namespace uome {

Client::Client() {
}

Client* Client::singleton_ = NULL;
Client* Client::getSingleton() {
    return singleton_;
}

int Client::sMain(const std::vector<CL_String8>& args) {
    singleton_ = new Client();
    int ret = singleton_->main(args);
    delete singleton_;
    return ret;
}

const boost::program_options::variables_map& Client::getConfig() const {
    return config_;
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

    ui::Manager* uiManager = uome::ui::Manager::getSingleton();
    boost::shared_ptr<CL_DisplayWindow> wnd = uiManager->getMainWindow();

    CL_GUITopLevelDescription desc(CL_Rect(10, 10, CL_Size(810, 610)), true);
    desc.set_decorations(false);

    ui::GumpMenu* ingameMenu = new ui::GumpMenu(desc);
    ingameMenu->setClosable(false);

    ui::IngameView* ingameView = new ui::IngameView(ingameMenu, CL_Rect(5, 5, CL_Size(800, 600)));
    ingameView->setCenterTiles(176 * 8, 202 * 8);


    ui::GumpMenu* testGump = ui::GumpFactory::fromXmlFile("simpletest");
    (void)testGump;

    timeval lastTime;
    gettimeofday(&lastTime, NULL);

    // elapsed milliseconds since the last cycle
    unsigned int elapsedMillis;

    // fps are calculated every 50 frames => sum time
    unsigned int fpsSum = 0;


    unsigned int i = 0;
    while (!wnd->get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE)) {
        ++i;

        timeval curTime;
        gettimeofday(&curTime, NULL);

        elapsedMillis = (curTime.tv_sec - lastTime.tv_sec) * 1000;
        elapsedMillis += (curTime.tv_usec - lastTime.tv_usec) / 1000;

        lastTime = curTime;

        fpsSum += elapsedMillis;

        if (i % 50 == 0) {
            float fps = fpsSum / 1000.0f; // seconds
            fps /= 50.0f; // 50 cycles
            fps = 1 / fps;

            std::ostringstream titleHelper;
            titleHelper << "UO:ME -- fps: " << std::setiosflags(std::ios::fixed) << std::setprecision(1) << fps;
            wnd->set_title(titleHelper.str());

            fpsSum = 0;
            //LOGARG_DEBUG(LOGTYPE_MAIN, "fps: %.1f", fps);
        }

        // adding sectors has to be done before sorting
        world::Manager::getSingleton()->getSectorManager()->addNewSectors();

        uiManager->getRenderQueue()->prepareRender((unsigned int)elapsedMillis);

        // deleting sectors has to be done after RenderQueue::prepareRender()
        // TODO: maybe before add?
        world::Manager::getSingleton()->getSectorManager()->deleteSectors();

        // call renderer
        uiManager->processMessages();
        uiManager->drawWindow();

        CL_KeepAlive::process();

        ui::Manager::getSingleton()->processCloseList();

        //CL_System::sleep(10);
    }

    // clean up
    ui::Manager::destroy();
    world::Manager::destroy();
    data::Manager::destroy();

    LOG_INFO(LOGTYPE_MAIN, "end of main");

    return 0;
}

}
