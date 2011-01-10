#ifndef UOME_MAP_LOADER_HPP
#define UOME_MAP_LOADER_HPP

#include "weakptrcache.hpp"
#include "fixedsizeondemandfileloader.hpp"

#include <boost/filesystem.hpp>


namespace uome {

namespace world {
    class MapBlock;
}

namespace data {

class MapLoader {
public:
    MapLoader(const boost::filesystem::path& mulPath, const boost::filesystem::path& difOffsetsPath, const boost::filesystem::path& difPath,
              unsigned int blockCountX, unsigned int blockCountY);

    MapLoader(const boost::filesystem::path& mulPath, unsigned int blockCountX, unsigned int blockCountY);

    void readCallbackMul(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<world::MapBlock> block, unsigned int extra, unsigned int userData);

    void readCallbackDifOffsets(int8_t* buf, unsigned int len);

    boost::shared_ptr<world::MapBlock> get(unsigned int x, unsigned int y);
    boost::shared_ptr<world::MapBlock> getNoCreate(unsigned int x, unsigned int y);

    unsigned int getBlockCountX();
    unsigned int getBlockCountY();

private:
    WeakPtrCache<world::MapBlock, FixedSizeOnDemandFileLoader> mulCache_;
    WeakPtrCache<world::MapBlock, FixedSizeOnDemandFileLoader> difCache_;

    // stores block idx - file offset in dif file pairs
    std::map<unsigned int, unsigned int> difEntries_;

    unsigned int blockCountX_;
    unsigned int blockCountY_;

    bool difEnabled_;

    void setSurroundingZ(boost::shared_ptr<world::MapBlock> item, int8_t zValues[][11]);
};

}
}


#endif
