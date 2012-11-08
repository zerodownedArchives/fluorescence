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
#include <boost/filesystem/fstream.hpp>

#include <misc/config.hpp>
#include <misc/log.hpp>
#include <misc/patcherupdater.hpp>

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
#include <net/packets/83_characterdelete.hpp>

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

void Client::selectShard(const UnicodeString& shardName) {
    config_.setShardName(shardName);
    setState(STATE_PRE_LOGIN);
}

void Client::disconnect() {
    net::Manager::getSingleton()->disconnect();
    setState(STATE_PRE_LOGIN);
}

bool Client::connect(const UnicodeString& host, unsigned int port, const UnicodeString& account, const UnicodeString& password) {
    return net::Manager::getSingleton()->connect(host, port, account, password);
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
            LOG_INFO << "Selected shard: " << config_.getShardName() << std::endl;

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
        //uiManager->openPythonGump("test");
        uiManager->openPythonGump("login");

        break;

    case STATE_PLAYING:
        uiManager->openPythonGump("gamewindow");
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
    if (config_.isLoaded()) {
        LOG_INFO << "Saving config" << std::endl;
        boost::filesystem::path tempPath("config.xml.tmp");
        if (config_.save(tempPath, false)) {
            boost::filesystem::path path = config_.getShardPath() / "config.xml";
            boost::filesystem::path backupPath = config_.getShardPath() / "config.xml.backup";

            if (boost::filesystem::exists(backupPath)) {
                boost::filesystem::remove(backupPath);
            }

            if (boost::filesystem::exists(path)) {
                boost::filesystem::rename(path, backupPath);
            }


            boost::filesystem::rename(tempPath, path);
        }
    }
}

bool Client::initBase(const std::vector<CL_String8>& args) {
    LOG_INFO << "Parsing command line" << std::endl;
    if (!config_.parseCommandLine(args)) {
        return false;
    }

    LOG_INFO << "Initializing basic data loaders" << std::endl;
    if (!data::Manager::create()) {
        cleanUp();
        return false;
    }

    LOG_INFO << "Initializing basic ui" << std::endl;
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

    LOG_INFO << "Setting up data loaders" << std::endl;
    // TODO: change from try/catch to boolean return only
    try {
        if (!data::Manager::getSingleton()->setShardConfig(config_)) {
            return false;
        }
    } catch (std::exception& e) {
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

    config_.initDefaults();

    // patcher can't update itself directly => make sure the new files are copied to the main folder
    PatcherUpdater::copyPatcherUpdateFolder();

    if (std::find(args.begin(), args.end(), "--patcherupdate") != args.end()) {
        // patcher received an update. restart it?
        // => not really necessary now, maybe when it does some more complex stuff
    }

    if (!initBase(args)) {
        cleanUp();
        return 1;
    }

    // if we already have a command line argument, take it
    if (config_.isLoaded()) {
        setState(STATE_PRE_LOGIN);
    } else {
        ui::GumpMenus::openShardList();
        LOG_INFO << "Selecting shard through user interface" << std::endl;
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

bool Client::createShard(const UnicodeString& name, const UnicodeString& pathStr, bool highSeas) {
    boost::filesystem::path path(StringConverter::toUtf8String(pathStr));

    boost::filesystem::path clientPath = path / "client.exe";
    if (!boost::filesystem::exists(clientPath)) {
        clientPath = path / "Client.exe";
        if (!boost::filesystem::exists(clientPath)) {
            LOG_ERROR << "Invalid Ultima Online directory" << std::endl;
            ui::GumpMenus::openMessageBox("Invalid Ultima Online directory");
            return false;
        }
    }

    boost::filesystem::path shardPath(StringConverter::toUtf8String(name));
    shardPath = "shards" / shardPath;
    if (boost::filesystem::exists(shardPath)) {
        LOG_ERROR << "Shard already exists" << std::endl;
        ui::GumpMenus::openMessageBox("Shard already exists");
        return false;
    }

    try {
        if (!boost::filesystem::create_directory(shardPath)) {
            LOG_ERROR << "Failed to create shard directory: " << std::endl;
            ui::GumpMenus::openMessageBox("Failed to create shard directory");
            return false;
        }
    } catch (std::exception& ex) {
        LOG_ERROR << "Failed to create shard directory: " << ex.what() << std::endl;
        ui::GumpMenus::openMessageBox("Failed to create shard directory");
        return false;
    }

    boost::filesystem::path configPath = shardPath / "config.xml";
    boost::filesystem::ofstream configStream(configPath);
    if (!configStream) {
        LOG_ERROR << "Failed to create shard config file: " << std::endl;
        ui::GumpMenus::openMessageBox("Failed to create shard config file");
        boost::filesystem::remove(shardPath);
        return false;
    } else {
        configStream << "<?xml version=\"1.0\"?>\n<fluo>\n<files";
        if (highSeas) {
            configStream << " format=\"mul-hs\"";
        }
        configStream << ">\n<mul-directory path=\"" << path.string();
        configStream << "\" />\n</files>\n</fluo>";
        configStream.close();
    }

    boost::filesystem::path macroPath = shardPath / "macros.xml";
    boost::filesystem::ofstream macroStream(macroPath);
    if (!macroStream) {
        LOG_ERROR << "Failed to create macros file: " << std::endl;
        ui::GumpMenus::openMessageBox("Failed to create macros file");
        boost::filesystem::remove(shardPath);
        return false;
    } else {
        macroStream << "<?xml version=\"1.0\"?>\n<macros>\n";
        macroStream << "<macro key=\"return\">\n\t<command name=\"speechentry\" />\n</macro>\n";
        macroStream << "<macro key=\"q\" ctrl=\"true\">\n\t<command name=\"effect\" param=\"countdown\" />\n</macro>\n";
        macroStream << "<macro key=\"w\" ctrl=\"true\">\n\t<command name=\"effect\" param=\"badsmell\" />\n</macro>\n";
        macroStream << "<macro key=\"e\" ctrl=\"true\">\n\t<command name=\"effect\" param=\"deadlyfog\" />\n</macro>\n";
        macroStream << "<macro key=\"r\" ctrl=\"true\">\n\t<command name=\"effect\" param=\"explosion\" />\n</macro>\n";
        macroStream << "<macro key=\"t\" ctrl=\"true\">\n\t<command name=\"effect\" param=\"rain\" />\n</macro>\n";
        macroStream << "<macro key=\"a\" ctrl=\"true\">\n\t<command name=\"directionlight\" param=\"on\" />\n</macro>\n";
        macroStream << "<macro key=\"s\" ctrl=\"true\">\n\t<command name=\"directionlight\" param=\"off\" />\n</macro>\n";
        macroStream << "<macro key=\"s\" ctrl=\"true\">\n\t<command name=\"directionlight\" param=\"off\" />\n</macro>\n";
        macroStream << "<macro key=\"add\">\n\t<command name=\"zoom\" param=\"in\" />\n</macro>\n";
        macroStream << "<macro key=\"subtract\">\n\t<command name=\"zoom\" param=\"out\" />\n</macro>\n";
        macroStream << "<macro key=\"multiply\">\n\t<command name=\"zoom\" param=\"reset\" />\n</macro>\n";
        macroStream << "<macro key=\"tab\">\n\t<command name=\"togglewarmode\" />\n</macro>\n";
        macroStream << "<macro key=\"escape\">\n\t<command name=\"cancel\" />\n</macro>\n";
        macroStream << "</macros>";
        macroStream.close();
    }

    selectShard(name);
    return true;
}

}
