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

    void read(int8_t* buf, unsigned int len);

private:
    unsigned int size_;
    IndexBlock* indexBlocks_;

};

}
}

#endif
