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



#include "equipconvdefloader.hpp"
#include "deffileloader.hpp"

namespace fluo {
namespace data {

EquipConvDefLoader::EquipConvDefLoader() {
}

EquipConvDefLoader::EquipConvDefLoader(const boost::filesystem::path& path) {
    DefFileLoader<EquipConvDef> defLoader(path, "iiiii");

    std::map<int, EquipConvDef>::const_iterator iter = defLoader.begin();
    std::map<int, EquipConvDef>::const_iterator end = defLoader.end();

    for (; iter != end; ++iter) {
        EquipConvDef curValue = iter->second;
        std::pair<unsigned int, unsigned int> id = std::make_pair(curValue.bodyId_, curValue.gumpId_);
        //LOG_DEBUG << "store id " << id << std::endl;
        table_[id] = curValue;
    }
}

bool EquipConvDefLoader::hasValue(unsigned int bodyId, unsigned int itemId) const {
    std::pair<unsigned int, unsigned int> id = std::make_pair(bodyId, itemId);
    return table_.find(id) != table_.end();
}

EquipConvDef EquipConvDefLoader::get(unsigned int bodyId, unsigned int itemId) const {
    std::pair<unsigned int, unsigned int> id = std::make_pair(bodyId, itemId);
    std::map<std::pair<unsigned int, unsigned int>, EquipConvDef>::const_iterator iter = table_.find(id);
    if (iter != table_.end()) {
        return iter->second;
    } else {
        return EquipConvDef();
    }
}

}
}
