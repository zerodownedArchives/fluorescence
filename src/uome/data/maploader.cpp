
#include "maploader.hpp"
#include "fullfileloader.hpp"

#include <boost/bind.hpp>

namespace uome {
namespace data {

MapLoader::MapLoader(const boost::filesystem::path& mulPath, const boost::filesystem::path& difIndexPath, const boost::filesystem::path& difPath,
                     unsigned int blockCountX, unsigned int blockCountY):
        blockCountX_(blockCountX), blockCountY_(blockCountY) {

    FullFileLoader difIndexLoader(difIndexPath);
    difIndexLoader.read(boost::bind(&MapLoader::readCallbackDifIdx, this, _1, _2));

    boost::shared_ptr<FixedSizeOnDemandFileLoader<world::MapBlock> > mulStream(new FixedSizeOnDemandFileLoader<world::MapBlock>(mulPath, 196,
            boost::bind(&MapLoader::readCallbackMul, this, _1, _2, _3, _4, _5)));
    boost::shared_ptr<FixedSizeOnDemandFileLoader<world::MapBlock> > difStream(new FixedSizeOnDemandFileLoader<world::MapBlock>(difPath, 196,
            boost::bind(&MapLoader::readCallbackMul, this, _1, _2, _3, _4, _5)));
    mulCache_.init(mulStream);
    difCache_.init(difStream);
}

void MapLoader::readCallbackMul(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<world::MapBlock> item, unsigned int extra) {
    unsigned int blockX = index % 512;
    unsigned int blockY = index / 512;

    item->blockIndexX = blockX;
    item->blockIndexX = blockY;

    unsigned int cellXOffset = blockX * 8;
    unsigned int cellYOffset = blockY * 8;

    uint16_t artId;
    int8_t z;

    // jump header
    buf += 4;

    for (unsigned int cellX = 0; cellX < 8; ++cellX) {
        for (unsigned int cellY = 0; cellY < 8; ++cellY) {
            artId = *(reinterpret_cast<uint16_t*>(buf));
            z = *(buf + 2);
            buf += 3;

            item->tiles_[(cellY * 8) + cellX].set(cellX + cellXOffset, cellY + cellYOffset, z, artId);
        }
    }
}

void MapLoader::readCallbackDifIdx(int8_t* buf, unsigned int len) {
    unsigned int entryCount = len / 4;
    uint32_t* ptr = reinterpret_cast<uint32_t*>(buf);

    for (unsigned int i = 0; i < entryCount; ++i) {
        unsigned int curEntry = *ptr;
        ptr++;
        difEntries_[curEntry] = i;
    }
}

boost::shared_ptr<world::MapBlock> MapLoader::get(unsigned int x, unsigned int y) {
    if (x >= blockCountX_) {
        x = 0;
    }

    if (y >= blockCountY_) {
        y = 0;
    }

    unsigned int idx = x * blockCountY_ + y;

    // check if there is a dif entry for this block
    std::map<unsigned int, unsigned int>::const_iterator iter = difEntries_.find(idx);
    if (iter != difEntries_.end()) {
        return difCache_.get(iter->second);
    } else {
        return mulCache_.get(idx);
    }
}

}
}
