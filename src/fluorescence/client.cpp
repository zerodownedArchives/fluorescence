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



#include "client.hpp"

#include <iostream>
#include <iomanip>

#include <misc/config.hpp>
#include <misc/log.hpp>

#include <ui/manager.hpp>
#include <ui/gumpmenus.hpp>
#include <ui/render/renderqueue.hpp>
#include <ui/gumpmenu.hpp>
#include <ui/gumpactions.hpp>

#include <data/manager.hpp>

#include <world/manager.hpp>
#include <world/sectormanager.hpp>
#include <world/sector.hpp>

#include <net/manager.hpp>
#include <net/packets/characterselect.hpp>
#include <net/packets/characterdelete.hpp>

namespace fluo {

Client::Client() : state_(STATE_SHARD_SELECTION), requestedState_(STATE_SHARD_SELECTION) {
    gettimeofday(&startTime_, NULL);
}

Client* Client::singleton_ = NULL;
Client* Client::getSingleton() {
    return singleton_;
}

int Client::sMain(const std::vector<CL_String8>& args) {
    singleton_ = new Client();
    LOG_INIT(0);

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

bool Client::shutdown(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    shutdown();
    return true;
}

bool Client::selectShard(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    selectShard(parameters[0]);
    return true;
}

void Client::selectShard(const UnicodeString& shardName) {
    config_["/fluo/shard@name"].setString(shardName);
    setState(STATE_PRE_LOGIN);
}

bool Client::disconnect(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    disconnect();
    return true;
}

void Client::disconnect() {
    net::Manager::getSingleton()->disconnect();
    setState(STATE_PRE_LOGIN);
}

void Client::setState(unsigned int value) {
    requestedState_ = value;
}

unsigned int Client::getState() {
    return state_;
}

bool Client::handleStateChange() {
    ui::Manager* uiManager = fluo::ui::Manager::getSingleton();

    // end old state
    switch (state_) {
    case STATE_SHARD_SELECTION:
        if (requestedState_ != STATE_SHUTDOWN) {
            LOG_INFO << "Selected shard: " << config_["/fluo/shard@name"].asString() << std::endl;

            if (!initFull()) {
                return false;
            }
        }
        break;

    case STATE_PLAYING:
        uiManager->uninstallMacros();
        uiManager->closeAllNonMessageGumps();
        uiManager->releaseIngameObjects();
        world::Manager::getSingleton()->clear();

        break;
    }


    // start new state
    switch(requestedState_) {
    case STATE_PRE_LOGIN:
        ui::GumpMenus::openLoginGump();

        break;

    case STATE_PLAYING:
        uiManager->openXmlGump("gamewindow");
        uiManager->installMacros();

        break;
    }

    return true;
}

void Client::cleanUp() {
    ui::Manager::getSingleton()->destroyAllGumpMenus();
    ui::Manager::getSingleton()->releaseIngameObjects();
    net::Manager::destroy();
    world::Manager::destroy();
    data::Manager::destroy();
    ui::Manager::destroy();
}

void Client::saveConfig() {
    if (config_.exists("/fluo/shard@name")) {
        LOG_INFO << "Saving config" << std::endl;
        boost::filesystem::path tempPath = "config.xml.tmp";
        if (config_.save(tempPath, false)) {
            boost::filesystem::path path = "shards" / config_["/fluo/shard@name"].asPath() / "config.xml";
            boost::filesystem::path backupPath = "shards" / config_["/fluo/shard@name"].asPath() / "config.xml.backup";

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

    ui::GumpActions::buildBasicActionTable();

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

    LOG_INFO << "Setting up gump event handlers" << std::endl;
    ui::GumpActions::buildFullActionTable();

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
        titleHelper << "fluorescence -- fps: " << std::setiosflags(std::ios::fixed) << std::setprecision(1) << fps;
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
    setlocale(LC_NUMERIC, "C");

    if (!initBase(args)) {
        cleanUp();
        return 1;
    }

    //UnicodeString selectedShard;
    // if we already have a command line argument, take it
    if (config_.exists("/fluo/shard@name")) {
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
    static ui::Manager* uiManager = ui::Manager::getSingleton();
    static net::Manager* netManager = net::Manager::getSingleton();
    static world::Manager* worldManager = world::Manager::getSingleton();

    netManager->step();
    uiManager->stepInput(elapsedMillis);
    uiManager->stepAudio();
    worldManager->step(elapsedMillis);
    uiManager->stepDraw();
}

void Client::doStateShardSelection() {
    static ui::Manager* uiManager = ui::Manager::getSingleton();

    uiManager->stepInput(0);
    uiManager->stepDraw();

    CL_System::sleep(10);
}

void Client::doStatePreLogin() {
    static ui::Manager* uiManager = ui::Manager::getSingleton();

    uiManager->stepInput(0);
    uiManager->stepAudio();
    uiManager->stepDraw();

    CL_System::sleep(10);
}

void Client::doStateLogin() {
    static ui::Manager* uiManager = ui::Manager::getSingleton();
    static net::Manager* netManager = net::Manager::getSingleton();

    netManager->step();
    uiManager->stepInput(0);
    uiManager->stepAudio();
    uiManager->stepDraw();

    CL_System::sleep(10);
}

bool Client::selectCharacter(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    net::packets::CharacterSelect reply(parameters[2], parameters[1], StringConverter::toInt(parameters[0]), net::Manager::getSingleton()->getSeed());
    net::Manager::getSingleton()->send(reply);

    return true;
}

bool Client::deleteCharacter(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    net::packets::CharacterDelete reply(parameters[1], StringConverter::toInt(parameters[0]), net::Manager::getSingleton()->getSeed());
    net::Manager::getSingleton()->send(reply);

    return true;
}

void Client::loginComplete() {
    setState(STATE_PLAYING);
}

timeval Client::getElapsedTime() const {
    timeval cur;
    gettimeofday(&cur, NULL);

    timeval t;
    t.tv_sec = cur.tv_sec - startTime_.tv_sec;

    if (cur.tv_usec >= startTime_.tv_usec) {
        t.tv_usec = cur.tv_usec - startTime_.tv_usec;
    } else {
        t.tv_sec -= 1;
        t.tv_usec = (cur.tv_usec + 1000000) - startTime_.tv_usec;
    }

    return t;
}

}
