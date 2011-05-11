
#include "client.hpp"

#include <iostream>
#include <iomanip>

#include <misc/config.hpp>
#include <misc/log.hpp>

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
#include <net/packets/characterselect.hpp>

namespace uome {

Client::Client() : state_(STATE_SHARD_SELECTION), requestedState_(STATE_SHARD_SELECTION) {
}

Client* Client::singleton_ = NULL;
Client* Client::getSingleton() {
    return singleton_;
}

int Client::sMain(const std::vector<CL_String8>& args) {
    LOG_INIT(0);

    singleton_ = new Client();
    int ret = singleton_->main(args);
    delete singleton_;

    LOG_CLOSE;
    return ret;
}

Config& Client::getConfig() {
    return config_;
}

void Client::shutdown() {
    setState(STATE_SHUTDOWN);
}

bool Client::shutdown(ui::GumpMenu* menu, ui::components::LocalButton* button) {
    shutdown();
    return true;
}

bool Client::selectShard(ui::GumpMenu* menu, ui::components::LocalButton* button) {
    config_["/uome/shard@name"].setString(button->getParameter());
    setState(STATE_PRE_LOGIN);
    //setState(STATE_PLAYING);
    return true;
}

bool Client::disconnect(ui::GumpMenu* menu, ui::components::LocalButton* button) {
    net::Manager::getSingleton()->disconnect();
    setState(STATE_PRE_LOGIN);
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
            LOG_INFO << "Selected shard: " << config_["/uome/shard@name"].asString() << std::endl;

            if (!initFull()) {
                return false;
            }
        }
        break;
    }


    // start new state
    switch(requestedState_) {
    case STATE_PRE_LOGIN:
        ui::GumpMenus::openLoginGump();

        //ui::Manager::getSingleton()->openXmlGump("simpletest");


        break;

    case STATE_PLAYING:
        ui::Manager* uiManager = uome::ui::Manager::getSingleton();
        boost::shared_ptr<CL_DisplayWindow> wnd = uiManager->getMainWindow();

        CL_GUITopLevelDescription desc(CL_Rect(10, 10, CL_Size(810, 610)), true);
        desc.set_decorations(false);

        ui::GumpMenu* ingameMenu = new ui::GumpMenu(desc);
        ingameMenu->setClosable(false);

        ui::IngameView* ingameView = new ui::IngameView(ingameMenu, CL_Rect(5, 5, CL_Size(800, 600)));
        ingameView->setCenterObject(world::Manager::getSingleton()->getPlayer());
        //ingameView->setCenterTiles(176 * 8, 202 * 8);
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

void Client::saveConfig() {
    if (config_.exists("/uome/shard@name")) {
        LOG_INFO << "Saving config" << std::endl;
        boost::filesystem::path tempPath = "config.xml.tmp";
        if (config_.save(tempPath, false)) {
            boost::filesystem::path path = "shards" / config_["/uome/shard@name"].asPath() / "config.xml";
            boost::filesystem::path backupPath = "shards" / config_["/uome/shard@name"].asPath() / "config.xml.backup";

            if (boost::filesystem::exists(backupPath)) {
                boost::filesystem::remove(backupPath);
            }

            if (boost::filesystem::exists(path)) {
                boost::filesystem::rename(path, backupPath);
            }


            boost::filesystem::rename(tempPath, path);
        }

        //path = "fullConfig.xml";
        //config_.save(path, true);
    }
}

bool Client::initBase(const std::vector<CL_String8>& args) {
    LOG_INFO << "Parsing command line" << std::endl;
    if (!config_.parseCommandLine(args)) {
        return false;
    }

    LOG_INFO << "Initializing ui" << std::endl;
    if (!ui::Manager::create()) {
        cleanUp();
        return false;
    }
    ui::components::LocalButton::buildBasicActionTable();

    return true;
}

bool Client::initFull() {
    LOG_INFO << "Parsing shard config" << std::endl;
    if (!config_.parseShardConfig()) {
        return false;
    }

    //config_.dumpMap();

    LOG_INFO << "Creating data loaders" << std::endl;
    if (!data::Manager::create(config_)) {
        return false;
    }

    LOG_INFO << "Setting up ui" << std::endl;
    if (!ui::Manager::getSingleton()->setShardConfig(config_)) {
        return false;
    }

    LOG_INFO << "Initializing world" << std::endl;
    if (!world::Manager::create(config_)) {
        return false;
    }

    LOG_INFO << "Initializing network" << std::endl;
    if (!net::Manager::create(config_)) {
        return false;
    }

    LOG_INFO << "Setting up event handlers" << std::endl;
    ui::components::LocalButton::buildFullActionTable();

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
        //LOGARG_DEBUG << "fps: " <<  fps << std::endl;
    }

    return fps;
}

int Client::main(const std::vector<CL_String8>& args) {
    LOG_INFO << "Client::main" << std::endl;
    setlocale(LC_ALL, "");

    if (!initBase(args)) {
        cleanUp();
        return 1;
    }

    //UnicodeString selectedShard;
    // if we already have a command line argument, take it
    if (config_.exists("/uome/shard@name")) {
        setState(STATE_PRE_LOGIN);
    } else {
        if (!ui::GumpMenus::openShardSelectionGump()) {
            LOG_EMERGENCY << "No shard chosen, and unable to select through ui" << std::endl;
            cleanUp();
            return 1;
        } else {
            LOG_INFO << "Selecting shard through user interface" << std::endl;
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
            doStateShardSelection();
            break;

        case STATE_PRE_LOGIN:
            doStatePreLogin();
            break;

        case STATE_LOGIN:
            doStateLogin();
            break;

        case STATE_PLAYING:
            calculateFps(elapsedMillis);
            doStatePlaying(elapsedMillis);
            break;
        }
    }

    cleanUp();

    saveConfig();

    LOG_INFO << "end of Client::main" << std::endl;

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

void Client::doStateShardSelection() {
    static ui::Manager* uiManager = ui::Manager::getSingleton();

    uiManager->step();

    CL_System::sleep(10);
}

void Client::doStatePreLogin() {
    static ui::Manager* uiManager = ui::Manager::getSingleton();

    uiManager->step();

    CL_System::sleep(10);
}

void Client::doStateLogin() {
    static ui::Manager* uiManager = ui::Manager::getSingleton();
    static net::Manager* netManager = net::Manager::getSingleton();

    netManager->step();
    uiManager->step();

    CL_System::sleep(10);
}

bool Client::selectCharacter(ui::GumpMenu* menu, ui::components::LocalButton* button) {
    net::packets::CharacterSelect reply(button->getText(), button->getParameter(1), StringConverter::toInt(button->getParameter(0)), net::Manager::getSingleton()->getSeed());
    net::Manager::getSingleton()->send(reply);

    return true;
}

void Client::loginComplete() {
    setState(STATE_PLAYING);
}

}
