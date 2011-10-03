
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

unsigned int MobTypesLoader::getIdleAction(unsigned int bodyId) {
    switch (getType(bodyId)) {
        case TYPE_ANIMAL:
            return 2;
        case TYPE_SEA_MONSTER: // TODO: is this correct?
        case TYPE_MONSTER:
            return 1;
        case TYPE_HUMAN:
        case TYPE_EQUIPMENT:
            // TODO: check mount and warmode
        default:
            return 4;
    }
}

}
}
