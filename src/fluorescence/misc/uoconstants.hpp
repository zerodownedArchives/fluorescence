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


#ifndef FLUO_MISC_UOCONSTANTS_HPP
#define FLUO_MISC_UOCONSTANTS_HPP

namespace fluo {
namespace misc {

struct Direction {
    enum {
        NORTH = 0x00,
        NORTHEAST = 0x01,
        EAST = 0x02,
        SOUTHEAST = 0x03,
        SOUTH = 0x04,
        SOUTHWEST = 0x05,
        WEST = 0x06,
        NORTHWEST = 0x07,
    }
};
};
}


#endif
