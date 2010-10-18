#ifndef UOME_DATA_INDEXLOADER_HPP
#define UOME_DATA_INDEXLOADER_HPP

#include <boost/filesystem.hpp>

#include <exception.hpp>

namespace uome {
namespace data {

struct IndexBlock {
    int32_t offset_;
    int32_t length_;
    int32_t extra_;
};

class IndexLoader {
public:
    IndexLoader(const boost::filesystem::path& path) throw(Exception);

    IndexBlock* get(unsigned int id);

    void read(int8_t* buf, unsigned int len) throw(Exception);

private:
    unsigned int size_;
    IndexBlock* indexBlocks_;

};

}
}

#endif
