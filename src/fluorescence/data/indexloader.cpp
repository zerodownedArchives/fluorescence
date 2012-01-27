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



#include "indexloader.hpp"
#include "fullfileloader.hpp"

#include <misc/log.hpp>

#include <boost/bind.hpp>

namespace fluo {
namespace data {

IndexLoader::IndexLoader(const boost::filesystem::path& path) : indexBlocks_(NULL) {
    FullFileLoader ldr(path);
    ldr.read(boost::bind(&IndexLoader::read, this, _1, _2));
}

IndexLoader::~IndexLoader() {
    if (indexBlocks_) {
        delete [] indexBlocks_;
        indexBlocks_ = NULL;
    }
}

void IndexLoader::read(int8_t* buf, unsigned int len) {
    if ((len % 12) != 0) {
        LOG_WARN << "File size is not a multiple of IndexBlock" << std::endl;
    }

    size_ = (len / 12);
    LOG_DEBUG << "Reading index file size " << size_ << std::endl;
    indexBlocks_ = new IndexBlock[size_];

    int32_t* ptr = reinterpret_cast<int32_t*>(buf);

    for (unsigned int i = 0; i < size_; ++i) {
        indexBlocks_[i].offset_ = *ptr;
        ++ptr;
        indexBlocks_[i].length_ = *ptr;
        ++ptr;
        indexBlocks_[i].extra_ = *ptr;
        ++ptr;
    }
}

const IndexBlock& IndexLoader::get(unsigned int id) const {
    // if someone tries to load an unknown id, just return the first entry
    if (id > size_) {
        LOG_WARN << "Trying to access out of bounds index=" << id << " size=" << size_ << std::endl;
        id = 0;
    }

    return indexBlocks_[id];
}

}
}
