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


#ifndef FLUO_STATICS_LOADER_HPP
#define FLUO_STATICS_LOADER_HPP

#include "weakptrcache.hpp"
#include "indexedondemandfileloader.hpp"

#include <world/statics.hpp>

#include <boost/filesystem.hpp>

namespace fluo {
namespace data {

class StaticsLoader {
public:
    StaticsLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath,
                  const boost::filesystem::path& difOffsetsPath, const boost::filesystem::path& difIdxPath, const boost::filesystem::path& difPath,
                  unsigned int blockCountX, unsigned int blockCountY);

    StaticsLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath,
                  unsigned int blockCountX, unsigned int blockCountY);

    void readCallbackMul(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<world::StaticBlock> block, unsigned int extra, unsigned int userData);

    void readCallbackDifOffsets(int8_t* buf, unsigned int len);

    boost::shared_ptr<world::StaticBlock> get(unsigned int x, unsigned int y);


private:
    WeakPtrCache<unsigned int, world::StaticBlock, IndexedOnDemandFileLoader> mulCache_;
    WeakPtrCache<unsigned int, world::StaticBlock, IndexedOnDemandFileLoader> difCache_;

    // stores block idx - file offset in dif file pairs
    std::map<unsigned int, unsigned int> difEntries_;

    unsigned int blockCountX_;
    unsigned int blockCountY_;

    bool difEnabled_;
};

}
}


#endif
