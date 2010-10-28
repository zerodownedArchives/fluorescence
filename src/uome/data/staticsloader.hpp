#ifndef UOME_STATICS_LOADER_HPP
#define UOME_STATICS_LOADER_HPP

#include "weakptrcache.hpp"
#include "indexedondemandfileloader.hpp"

#include <world/statics.hpp>

#include <boost/filesystem.hpp>

namespace uome {
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
    WeakPtrCache<world::StaticBlock, IndexedOnDemandFileLoader> mulCache_;
    WeakPtrCache<world::StaticBlock, IndexedOnDemandFileLoader> difCache_;

    // stores block idx - file offset in dif file pairs
    std::map<unsigned int, unsigned int> difEntries_;

    unsigned int blockCountX_;
    unsigned int blockCountY_;

    bool difEnabled_;
};

}
}


#endif
