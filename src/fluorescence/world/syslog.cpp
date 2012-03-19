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

#include "syslog.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace world {
    
SysLogEntry::SysLogEntry(const UnicodeString& text, unsigned int hue, unsigned int font) :
        hue_(hue), font_(font), millisToLive_(5000), text_(text) {
    // make millisToLive dynamic?
}

SysLog::SysLog() : newEntry_(false) {
}

void SysLog::add(const UnicodeString& text, unsigned int hue, unsigned int font) {
    LOG_INFO << "SysMsg: " << text << std::endl;
    
    entries_.push_back(SysLogEntry(text, hue, font));
    
    newEntry_ = true;
}

void SysLog::update(unsigned int elapsedMillis) {
    if (entries_.empty()) {
        return;
    }
    
    std::list<SysLogEntry>::iterator iter = entries_.begin();
    std::list<SysLogEntry>::iterator end = entries_.end();
    
    bool doRemove = false;
    for (; iter != end; ++iter) {
        if (iter->millisToLive_ <= elapsedMillis) {
            iter->millisToLive_ = 0;
            doRemove = true;
        } else {
            iter->millisToLive_ -= elapsedMillis;
        }
    }
    
    if (doRemove) {
        entries_.remove_if(&SysLog::expiredRemoveHelper);
    }
    
    if ((doRemove || newEntry_) && !notifyObjects_.empty()) {
        // update registered components
        std::list<SysLogNotify*>::iterator iterFunc = notifyObjects_.begin();
        std::list<SysLogNotify*>::iterator endFunc = notifyObjects_.end();
        
        for (; iterFunc != endFunc; ++iterFunc) {
            (*iterFunc)->notify(entries_.begin(), entries_.end());
        }
    }
    
    newEntry_ = false;
}

bool SysLog::expiredRemoveHelper(const SysLogEntry& entry) {
    return entry.millisToLive_ == 0;
}

void SysLog::registerNotify(SysLogNotify* notify) {
    notifyObjects_.push_back(notify);
}

void SysLog::unregisterNotify(SysLogNotify* notify) {
    notifyObjects_.remove(notify);
}

}
}

