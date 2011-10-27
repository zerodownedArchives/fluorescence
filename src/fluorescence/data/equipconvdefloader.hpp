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

    bool hasValue(std::pair<unsigned int, unsigned int> id) const;

    EquipConvDef get(std::pair<unsigned int, unsigned int> id) const;

private:
    std::map<std::pair<unsigned int, unsigned int>, EquipConvDef> table_;

};
}
}


#endif
