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



#include "mobtypesloader.hpp"

#include <string.h>
#include <fstream>

namespace fluo {
namespace data {

void MobTypesLoader::parseType(MobTypeDef& def, unsigned int strIndex, const char* str, int*& ptr) {
    if (strncmp(str, "MONSTER", 7) == 0) {
        def.mobType_ = TYPE_MONSTER;
    } else if (strncmp(str, "ANIMAL", 6) == 0) {
        def.mobType_ = TYPE_ANIMAL;
    } else if (strncmp(str, "HUMAN", 5) == 0) {
        def.mobType_ = TYPE_HUMAN;
    } else if (strncmp(str, "EQUIPMENT", 9) == 0) {
        def.mobType_ = TYPE_EQUIPMENT;
    } else if (strncmp(str, "SEA_MONSTER", 11) == 0) {
        def.mobType_ = TYPE_SEA_MONSTER;
    } else {
        LOG_ERROR << "Unknown type in mobtypes.txt: " << str << std::endl;
        def.mobType_ = TYPE_MONSTER;
    }
    
    ++ptr;
}

}
}
