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


#ifndef FLUO_DATA_INDEXLOADER_HPP
#define FLUO_DATA_INDEXLOADER_HPP

#include <boost/filesystem.hpp>

#include <misc/exception.hpp>

namespace fluo {
namespace data {

struct IndexBlock {
    uint32_t offset_;
    uint32_t length_;
    uint32_t extra_;
};

class IndexLoader {
public:
    IndexLoader(const boost::filesystem::path& path);
    ~IndexLoader();

    const IndexBlock& get(unsigned int id) const;

    unsigned int size() const;

    void read(int8_t* buf, unsigned int len);

private:
    unsigned int size_;
    IndexBlock* indexBlocks_;

    UnicodeString fileName_;

};

}
}

#endif
