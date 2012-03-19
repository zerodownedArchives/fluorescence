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


#ifndef FLUO_WORLD_SYSLOG_HPP
#define FLUO_WORLD_SYSLOG_HPP

#include <list>

#include <misc/string.hpp>

namespace fluo {
namespace world {
    
struct SysLogEntry {
    unsigned int hue_;
    unsigned int font_;
    unsigned int millisToLive_;
    
    UnicodeString text_;
    
    
    SysLogEntry(const UnicodeString& text, unsigned int hue, unsigned int font);
};

class SysLogNotify {
public:
    virtual void notify(std::list<SysLogEntry>::const_iterator begin, std::list<SysLogEntry>::const_iterator end) = 0;
};

class SysLog {
public:
    SysLog();
    
    void add(const UnicodeString& text, unsigned int hue, unsigned int font);
    
    void update(unsigned int elapsedMillis);
    
    void registerNotify(SysLogNotify* notify);
    void unregisterNotify(SysLogNotify* notify);

private:
    std::list<SysLogEntry> entries_;
    bool newEntry_;
    
    static bool expiredRemoveHelper(const SysLogEntry& entry);
    
    std::list<SysLogNotify*> notifyObjects_;
};

}
}

#endif

