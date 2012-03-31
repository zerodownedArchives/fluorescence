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


#ifndef FLUO_UI_MACROMANAGER_HPP
#define FLUO_UI_MACROMANAGER_HPP

#include <list>
#include <map>
#include <ClanLib/Display/Window/input_event.h>


#include <misc/config.hpp>

namespace fluo {
namespace ui {

class Macro {
public:
    Macro(unsigned int keyCode, bool shift, bool ctrl, bool alt);
    void addCommand(const UnicodeString& command, const UnicodeString& args);
    
    int getKeyCode() const;
    bool matchesEvent(const CL_InputEvent& event) const;
    
    void execute() const;
    
    bool isValid() const;
    UnicodeString toString() const;
    
private:
    int keyCode_;
    bool shift_;
    bool ctrl_;
    bool alt_;
    
    std::list<std::pair<UnicodeString, UnicodeString> > commands_;
};

class MacroManager {
public:
    MacroManager(Config& config);
    
    void init();
    void clear();
    
    // return true if the event was consumed (= a macro found)
    bool execute(const CL_InputEvent& event) const;
    
private:
    std::multimap<int, Macro> macros_;
};

}
}

#endif

