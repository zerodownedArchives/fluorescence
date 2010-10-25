#ifndef UOME_MAP_LOADER_HPP
#define UOME_MAP_LOADER_HPP

#include "weakptrcache.hpp"
#include "fixedsizeondemandfileloader.hpp"

#include <world/map.hpp>

#include <boost/filesystem.hpp>


namespace uome {
namespace data {

class MapLoader {
public:
    MapLoader(const boost::filesystem::path& mulPath, const boost::filesystem::path& difIndexPath, const boost::filesystem::path& difPath,
            unsigned int blockCountX, unsigned int blockCountY);

    void readCallbackMul(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<world::MapBlock> block, unsigned int extra);

    void readCallbackDifIdx(int8_t* buf, unsigned int len);

    boost::shared_ptr<world::MapBlock> get(unsigned int x, unsigned int y);


private:
    WeakPtrCache<world::MapBlock, FixedSizeOnDemandFileLoader> mulCache_;
    WeakPtrCache<world::MapBlock, FixedSizeOnDemandFileLoader> difCache_;

    // stores block idx - file offset in dif file pairs
    std::map<unsigned int, unsigned int> difEntries_;

    unsigned int blockCountX_;
    unsigned int blockCountY_;
};

}
}


#endif
