
#include "client.hpp"

#include <iostream>
#include <iomanip>

#include <misc/config.hpp>
#include <misc/logger.hpp>

#include <ui/manager.hpp>
#include <ui/gumpmenus.hpp>
#include <ui/renderqueue.hpp>
#include <ui/ingameview.hpp>
#include <ui/gumpmenu.hpp>
#include <ui/components/localbutton.hpp>

#include <data/manager.hpp>

#include <world/manager.hpp>
#include <world/sectormanager.hpp>
#include <world/sector.hpp>

#include <net/manager.hpp>




namespace uome {

Client::Client() : state_(STATE_SHARD_SELECTION), requestedState_(STATE_SHARD_SELECTION) {
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

Config* Client::getConfig() {
    return &config_;
}

void Client::shutdown() {
    setState(STATE_SHUTDOWN);
}

bool Client::shutdown(ui::GumpMenu* menu, const std::string& parameter) {
    shutdown();
    return true;
}

bool Client::selectShard(ui::GumpMenu* menu, const std::string& parameter) {
    config_.set("shard", parameter);
    setState(STATE_LOGIN);
    return true;
}

void Client::setState(unsigned int value) {
    requestedState_ = value;
}

bool Client::handleStateChange() {
    // end old state
    switch (state_) {
    case STATE_SHARD_SELECTION:
        if (requestedState_ != STATE_SHUTDOWN) {
            std::string selectedShard = config_["shard"].as<std::string>();
            LOGARG_INFO(LOGTYPE_MAIN, "Selected shard: %s", selectedShard.c_str());

            if (!initFull(selectedShard)) {
                return false;
            }
        }
        break;
    }


    // start new state
    switch(requestedState_) {
    case STATE_LOGIN:
        ui::GumpMenus::openLoginGump();
        break;

    case STATE_PLAYING:
        ui::Manager* uiManager = uome::ui::Manager::getSingleton();
        boost::shared_ptr<CL_DisplayWindow> wnd = uiManager->getMainWindow();

        CL_GUITopLevelDescription desc(CL_Rect(10, 10, CL_Size(810, 610)), true);
        desc.set_decorations(false);

        ui::GumpMenu* ingameMenu = new ui::GumpMenu(desc);
        ingameMenu->setClosable(false);

        ui::IngameView* ingameView = new ui::IngameView(ingameMenu, CL_Rect(5, 5, CL_Size(800, 600)));
        ingameView->setCenterTiles(176 * 8, 202 * 8);
        break;
    }

    return true;
}

void Client::cleanUp() {
    net::Manager::destroy();
    ui::Manager::destroy();
    world::Manager::destroy();
    data::Manager::destroy();
}

bool Client::initBase(const std::vector<CL_String8>& args) {
    LOG_INFO(LOGTYPE_MAIN, "Parsing command line");
    if (!config_.parseCommandLine(args)) {
        return false;
    }

    LOG_INFO(LOGTYPE_MAIN, "Initializing ui");
    if (!ui::Manager::create()) {
        cleanUp();
        return false;
    }
    ui::components::LocalButton::buildBasicActionTable();

    return true;
}

bool Client::initFull(const std::string& selectedShard) {
    LOG_INFO(LOGTYPE_MAIN, "Parsing shard config");
    if (!config_.parseShardConfig(selectedShard)) {
        return false;
    }

    LOG_INFO(LOGTYPE_MAIN, "Creating data loaders");
    if (!data::Manager::create(config_)) {
        return false;
    }

    LOG_INFO(LOGTYPE_MAIN, "Setting up ui");
    if (!ui::Manager::getSingleton()->setShardConfig(config_)) {
        return false;
    }

    LOG_INFO(LOGTYPE_MAIN, "Initializing world");
    if (!world::Manager::create(config_)) {
        return false;
    }

    LOG_INFO(LOGTYPE_MAIN, "Initializing network");
    if (!net::Manager::create(config_)) {
        return false;
    }

    LOG_INFO(LOGTYPE_MAIN, "Setting up event handlers");
    ui::components::LocalButton::buildFullActionTable();
    // init packet handler

    return true;
}


float Client::calculateFps(unsigned int elapsedMillis) {
    // fps are calculated every 100 frames => sum time
    static unsigned int fpsSum = 0;
    static unsigned int frameCount = 0;
    static float fps;

    ++frameCount;
    fpsSum += elapsedMillis;

    if (frameCount >= 100) {
        fps = fpsSum / 1000.0f; // seconds
        fps /= 100.0f; // 100 cycles
        fps = 1 / fps;

        std::ostringstream titleHelper;
        titleHelper << "UO:ME -- fps: " << std::setiosflags(std::ios::fixed) << std::setprecision(1) << fps;
        ui::Manager::getSingleton()->getMainWindow()->set_title(titleHelper.str());

        fpsSum = 0;
        frameCount = 0;
        //LOGARG_DEBUG(LOGTYPE_MAIN, "fps: %.1f", fps);
    }

    return fps;
}

int Client::main(const std::vector<CL_String8>& args) {
    LOG_INFO(LOGTYPE_MAIN, "Starting client");
    setlocale(LC_ALL, "");

    if (!initBase(args)) {
        cleanUp();
        return 1;
    }

    std::string selectedShard;
    // if we already have a command line argument, take it
    if (config_.count("shard") > 0) {
        setState(STATE_LOGIN);
    } else {
        if (!ui::GumpMenus::openShardSelectionGump()) {
            LOG_CRITICAL(LOGTYPE_MAIN, "No shard chosen, and unable to select through ui");
            cleanUp();
            return 1;
        } else {
            LOG_INFO(LOGTYPE_MAIN, "Selecting shard through user interface");
        }
    }

    timeval lastTime;
    gettimeofday(&lastTime, NULL);

    // elapsed milliseconds since the last cycle
    unsigned int elapsedMillis;

    while (state_ != STATE_SHUTDOWN) {
        timeval curTime;
        gettimeofday(&curTime, NULL);

        elapsedMillis = (curTime.tv_sec - lastTime.tv_sec) * 1000;
        elapsedMillis += (curTime.tv_usec - lastTime.tv_usec) / 1000;

        lastTime = curTime;

        // check for state change
        if (requestedState_ != state_) {
            if (!handleStateChange()) {
                setState(STATE_SHUTDOWN);
                continue;
            }
            state_ = requestedState_;
        }

        switch(state_) {
        case STATE_SHARD_SELECTION:
            doStateShardSelection(elapsedMillis);
            break;

        case STATE_LOGIN:
            doStateLogin(elapsedMillis);
            break;

        case STATE_PLAYING:
            calculateFps(elapsedMillis);
            doStatePlaying(elapsedMillis);
            break;
        }
    }

    cleanUp();

    LOG_INFO(LOGTYPE_MAIN, "end of main");
    return 0;
}

void Client::doStatePlaying(unsigned int elapsedMillis) {
    static world::Manager* worldManager = world::Manager::getSingleton();
    static ui::Manager* uiManager = ui::Manager::getSingleton();
    static net::Manager* netManager = net::Manager::getSingleton();

    netManager->step();

    worldManager->getSectorManager()->addNewSectors();
    uiManager->getRenderQueue()->prepareRender((unsigned int)elapsedMillis);
    // deleting sectors has to be done after RenderQueue::prepareRender() (list is sorted now)
    worldManager->getSectorManager()->deleteSectors();

    uiManager->step();
}

void Client::doStateShardSelection(unsigned int elapsedMillis) {
    static ui::Manager* uiManager = ui::Manager::getSingleton();

    uiManager->step();

    CL_System::sleep(10);
}

void Client::doStateLogin(unsigned int elapsedMillis) {
    static ui::Manager* uiManager = ui::Manager::getSingleton();
    static net::Manager* netManager = net::Manager::getSingleton();

    netManager->step();
    uiManager->step();

    CL_System::sleep(10);
}

}
