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

#include <misc/log.hpp>

namespace fluo {
namespace data {

MobTypesLoader::MobTypesLoader(const boost::filesystem::path& path) {
    memset(typeTable_, 0, 4096);

    std::ifstream ifs(path.string().c_str());
    if (!ifs.is_open() || !ifs.good()) {
        LOG_ERROR << "Unable to open mobtypes.txt from path: " << path.string() << std::endl;
        return;
    }

    char buf[1024];
    char* ptr;
    unsigned int line = 0;
    do {
        ++line;

        ifs.getline(buf, 1024);

        ptr = buf;

        // skip leading spaces
        while (*ptr == ' ' || *ptr == '\t') {
            ++ptr;
        }

        // skip comments and empty lines
        if (*ptr == '#' || *ptr == '\0' || *ptr == '\r' || *ptr == '\n') {
            continue;
        }

        int id = strtol(ptr, &ptr, 10);

        if (id == 0) {
            LOG_ERROR << "Failed to parse id in mobtypes.txt, line " << line << std::endl;
            continue;
        } else if (id >= 4096) {
            LOG_ERROR << "Id > 4096 in mobtypes.txt, line " << line << std::endl;
            continue;
        } else {
            // id okay,
            while (*ptr == ' ' || *ptr == '\t') {
                ++ptr;
            }

            if (*ptr == '#' || *ptr == '\0' || *ptr == '\r' || *ptr == '\n') {
                LOG_ERROR << "Incomplete line in mobtypes.txt, line " << line << std::endl;
                continue;
            }

            if (strncmp(ptr, "MONSTER", 7) == 0) {
                typeTable_[id] = TYPE_MONSTER;
            } else if (strncmp(ptr, "ANIMAL", 6) == 0) {
                typeTable_[id] = TYPE_ANIMAL;
            } else if (strncmp(ptr, "HUMAN", 5) == 0) {
                typeTable_[id] = TYPE_HUMAN;
            } else if (strncmp(ptr, "EQUIPMENT", 9) == 0) {
                typeTable_[id] = TYPE_EQUIPMENT;
            } else if (strncmp(ptr, "SEA_MONSTER", 11) == 0) {
                typeTable_[id] = TYPE_SEA_MONSTER;
            } else {
                LOG_ERROR << "Unknown type in mobtypes.txt, line " << line << std::endl;
                continue;
            }

            //LOG_DEBUG << "mobtypes id=" << id << " type=" << (unsigned int)typeTable_[id] << std::endl;
        }
    } while (ifs.good());

}

unsigned int MobTypesLoader::getType(unsigned int bodyId) {
    if (bodyId > 4096) {
        LOG_WARN << "MobTypesLoader::getType with too high body id: " << bodyId << std::endl;
        bodyId = 0;
    }

    return typeTable_[bodyId];
}

unsigned int MobTypesLoader::getIdleAction(unsigned int bodyId, bool mounted, bool warmodeOneHanded, bool warmodeTwoHanded) {
    switch (getType(bodyId)) {
        case TYPE_ANIMAL:
            return 2;
        case TYPE_SEA_MONSTER: // TODO: is this correct?
        case TYPE_MONSTER:
            return 1;
        case TYPE_HUMAN:
        case TYPE_EQUIPMENT:
        default:
            if (mounted) {
                return 25;
            } else if (warmodeOneHanded) {
                return 7;
            } else if (warmodeTwoHanded) {
                return 8;
            } else {
                return 4;
            }
    }
    
    LOG_WARN << "Unable to provide idle action for mobile " << bodyId << " mounted=" << mounted << 
            " warmodeOneHanded=" << warmodeOneHanded << " warmodeTwoHanded=" << warmodeTwoHanded << std::endl;
    return 4;
}

}
}
