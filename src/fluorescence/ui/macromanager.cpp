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

#include "macromanager.hpp"

#include <boost/filesystem/path.hpp>

#include "manager.hpp"
#include "commandmanager.hpp"

#include <data/manager.hpp>

namespace fluo {
namespace ui {
    
Macro::Macro(unsigned int keyCode, bool shift, bool ctrl, bool alt) : 
        keyCode_(keyCode), shift_(shift), ctrl_(ctrl), alt_(alt) {
}

void Macro::addCommand(const UnicodeString& command, const UnicodeString& args) {
    commands_.push_back(std::make_pair(command, args));
}

int Macro::getKeyCode() const {
    return keyCode_;
}

bool Macro::isValid() const {
    return commands_.size() > 0;
}

UnicodeString Macro::toString() const {
    UnicodeString str;
    if (shift_) {
        str += "SHIFT+";
    }
    if (ctrl_) {
        str += "CTRL+";
    }
    if (alt_) {
        str += "ALT+";
    }
    
    str += ui::Manager::getInputContext().get_keyboard().keyid_to_string(keyCode_).c_str();
    
    return str;
}
    
bool Macro::matchesEvent(const CL_InputEvent& event) const {
    return keyCode_ == event.id && event.alt == alt_ && event.shift == shift_ && event.ctrl == ctrl_;
}

void Macro::execute() const {
    std::list<std::pair<UnicodeString, UnicodeString> >::const_iterator iter = commands_.begin();
    std::list<std::pair<UnicodeString, UnicodeString> >::const_iterator end = commands_.end();
    
    for (; iter != end; ++iter) {
        ui::Manager::getCommandManager()->execute(iter->first, iter->second);
    }
}

    
MacroManager::MacroManager(Config& config) {
}

bool MacroManager::execute(const CL_InputEvent& event) const {
    bool ret = false;
    
    std::pair<std::multimap<int, Macro>::const_iterator, std::multimap<int, Macro>::const_iterator> range = macros_.equal_range(event.id);
    
    if (range.first != range.second) {
        std::multimap<int, Macro>::const_iterator iter = range.first;
        for (; iter != range.second; ++iter) {
            if (iter->second.matchesEvent(event)) {
                iter->second.execute();
                ret = true;
            }
        }
    }
    
    return ret;
}

void MacroManager::clear() {
    macros_.clear();
}

void MacroManager::init() {
    boost::filesystem::path path = data::Manager::getShardFilePath("macros.xml");
    LOG_INFO << "Loading macros from " << path << std::endl;
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.string().c_str());
    
    if (result) {
        for (pugi::xml_node macroIter = doc.document_element().first_child(); macroIter; macroIter = macroIter.next_sibling()) {
            if (strcmp(macroIter.name(), "macro") == 0) {
                const char* keyString = macroIter.attribute("key").value();
                int keyCode = ui::Manager::getInputContext().get_keyboard().string_to_keyid(keyString);
                
                if (keyCode == 0) {
                    continue;
                }
                
                bool shift = macroIter.attribute("shift").as_bool();
                bool ctrl = macroIter.attribute("ctrl").as_bool();
                bool alt = macroIter.attribute("alt").as_bool();
                
                Macro mac(keyCode, shift, ctrl, alt);
                
                for (pugi::xml_node cmdIter = macroIter.first_child(); cmdIter; cmdIter = cmdIter.next_sibling()) {
                    UnicodeString cmdStr = StringConverter::fromUtf8(cmdIter.attribute("name").value());
                    UnicodeString paramStr = StringConverter::fromUtf8(cmdIter.attribute("param").value());
                    
                    if (cmdStr.length() > 0) {
                        if (ui::Manager::getCommandManager()->hasCommand(cmdStr)) {
                            mac.addCommand(cmdStr, paramStr);
                            //LOG_INFO << "Adding command " << cmdStr << " to macro " << mac.toString() << std::endl;
                        } else {
                            LOG_WARN << "Unknown command " << cmdStr << " in macro " << mac.toString() << std::endl;
                        }
                    } else {
                        LOG_WARN << "Empty command in macro " << mac.toString() << std::endl;
                    }
                }
                
                if (mac.isValid()) {
                    macros_.insert(std::make_pair(keyCode, mac));
                    //LOG_INFO << "Installing macro: " << mac.toString() << std::endl;
                } else {
                    LOG_WARN << "Invalid macro: " << mac.toString() << std::endl;
                }
            } else {
                LOG_ERROR << "Unknown xml tag " << macroIter.name() << " in macros.xml" << std::endl;
            }
        }
    } else {
        LOG_ERROR << "Error parsing macros.xml file at offset " << result.offset << ": " << result.description() << std::endl;
    }
}

}
}

