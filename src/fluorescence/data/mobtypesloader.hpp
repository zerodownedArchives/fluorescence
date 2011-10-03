#ifndef FLUO_DATA_MOBTYPESLOADER_HPP
#define FLUO_DATA_MOBTYPESLOADER_HPP


#include <boost/filesystem/path.hpp>

namespace fluo {
namespace data {

class MobTypesLoader {
public:
    static const unsigned int TYPE_HUMAN = 0;
    static const unsigned int TYPE_MONSTER = 1;
    static const unsigned int TYPE_ANIMAL = 2;
    static const unsigned int TYPE_EQUIPMENT = 3;
    static const unsigned int TYPE_SEA_MONSTER = 4;

    MobTypesLoader(const boost::filesystem::path& path);

    unsigned int getType(unsigned int bodyId);
    unsigned int getIdleAction(unsigned int bodyId);

private:
    uint8_t typeTable_[4096];
};
}
}


#endif
