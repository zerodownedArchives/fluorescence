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


#ifndef FLUO_WORLD_BUFFINFO_HPP
#define FLUO_WORLD_BUFFINFO_HPP

#include <typedefs.hpp>
#include <misc/string.hpp>

namespace fluo {
namespace world {

struct BuffInfo {
    uint16_t buffType_;
    uint16_t sourceType_;
    uint16_t iconId_;
    uint16_t duration_;
    uint32_t clilocTitle_;
    uint32_t cliloc2_;
    uint32_t cliloc3_;
    UnicodeString clilocTitleArgs_;
    UnicodeString cliloc2Args_;
    UnicodeString cliloc3Args_;
};

struct BuffInfoRemoveHelper {
public:
    BuffInfoRemoveHelper(unsigned int t) : type_(t) { }

    bool operator() (const BuffInfo& info) { return info.buffType_ == type_; }
private:
    unsigned int type_;
};

}
}

#endif

