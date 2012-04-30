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

#include "commandmanager.hpp"

#include <misc/log.hpp>
#include <world/manager.hpp>

#include "commands/clientcommand.hpp"
#include "commands/say.hpp"
#include "commands/emote.hpp"
#include "commands/yell.hpp"
#include "commands/whisper.hpp"
#include "commands/command.hpp"
#include "commands/opengump.hpp"
#include "commands/disconnect.hpp"
#include "commands/effect.hpp"

namespace fluo {
namespace ui {
    
CommandManager::CommandManager(Config& config) {
    commandMap_["command"].reset(new commands::Command(config));
    
    commandMap_["say"].reset(new commands::Say(config));
    commandMap_["emote"].reset(new commands::Emote(config));
    commandMap_["whisper"].reset(new commands::Whisper(config));
    commandMap_["yell"].reset(new commands::Yell(config));
    commandMap_["opengump"].reset(new commands::OpenGump());
    commandMap_["disconnect"].reset(new commands::Disconnect());
    commandMap_["effect"].reset(new commands::Effect());
    
    // TODO: fill prefixes with values from config
    
    commandPrefix_ = '?';
    emotePrefix_ = ':';
    yellPrefix_ = '!';
    whisperPrefix_ = ';';
}

void CommandManager::execute(const UnicodeString& command, const UnicodeString& args) {
    std::map<UnicodeString, boost::shared_ptr<commands::ClientCommand> >::iterator iter = commandMap_.find(command);
    
    if (iter != commandMap_.end()) {
        iter->second->execute(args);
    } else {
        UnicodeString sysMsg("Unknown client command: ");
        sysMsg += command;
        world::Manager::getSingleton()->systemMessage(sysMsg);
        LOG_WARN << "Trying to execute unknown client command " << command << std::endl;
    }
}

void CommandManager::handleTextInput(const UnicodeString& text) {
    boost::shared_ptr<commands::ClientCommand> command;
    
    char cmdChar = text.charAt(0);
    bool removeFirstChar = true;
    if (cmdChar == commandPrefix_) {
        command = commandMap_["command"];
    } else if (cmdChar == emotePrefix_) {
        command = commandMap_["emote"];
    } else if (cmdChar == yellPrefix_) {
        command = commandMap_["yell"];
    } else if (cmdChar == whisperPrefix_) {
        command = commandMap_["whisper"];
    } else {
        command = commandMap_["say"];
        removeFirstChar = false;
    }
    
    if (removeFirstChar) {
        UnicodeString msg(text, 1);
        command->execute(msg.trim());
    } else {
        command->execute(text);
    }
}

bool CommandManager::hasCommand(const UnicodeString& cmd) const {
    return commandMap_.find(cmd) != commandMap_.end();
}
    
}
}
