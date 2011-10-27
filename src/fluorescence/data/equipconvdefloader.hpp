#ifndef FLUO_DATA_EQUIPCONVDEFLOADER_HPP
#define FLUO_DATA_EQUIPCONVDEFLOADER_HPP

#include <map>
#include <boost/filesystem/path.hpp>

#include "defstructs.hpp"

namespace fluo {
namespace data {

class EquipConvDefLoader {
public:
    EquipConvDefLoader(const boost::filesystem::path& path);

    bool hasValue(unsigned int bodyId, unsigned int itemId) const;

    EquipConvDef get(unsigned int bodyId, unsigned int itemId) const;

private:
    std::map<std::pair<unsigned int, unsigned int>, EquipConvDef> table_;

};
}
}


#endif
