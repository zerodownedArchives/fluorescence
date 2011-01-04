
#include "indexloader.hpp"
#include "fullfileloader.hpp"

#include <misc/logger.hpp>

#include <boost/bind.hpp>

namespace uome {
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
        throw Exception("File size is not a multiple of IndexBlock");
    }

    size_ = (len / 12);
    LOGARG_DEBUG(LOGTYPE_DATA, "Reading index file size %u", size_);
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
        LOGARG_WARN(LOGTYPE_DATA, "Trying to access out of bounds index %u, size %u", id, size_);
        id = 0;
    }

    return indexBlocks_[id];
}

}
}
