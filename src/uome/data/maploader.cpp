
#include "maploader.hpp"
#include "fullfileloader.hpp"

#include <boost/bind.hpp>

namespace uome {
namespace data {

MapLoader::MapLoader(const boost::filesystem::path& mulPath, const boost::filesystem::path& difOffsetsPath, const boost::filesystem::path& difPath,
                     unsigned int blockCountX, unsigned int blockCountY):
        blockCountX_(blockCountX), blockCountY_(blockCountY), difEnabled_(true) {

    FullFileLoader difOffsetsLoader(difOffsetsPath);
    difOffsetsLoader.read(boost::bind(&MapLoader::readCallbackDifOffsets, this, _1, _2));

    boost::shared_ptr<FixedSizeOnDemandFileLoader<world::MapBlock> > mulStream(new FixedSizeOnDemandFileLoader<world::MapBlock>(mulPath, 196,
            boost::bind(&MapLoader::readCallbackMul, this, _1, _2, _3, _4, _5, _6)));
    boost::shared_ptr<FixedSizeOnDemandFileLoader<world::MapBlock> > difStream(new FixedSizeOnDemandFileLoader<world::MapBlock>(difPath, 196,
            boost::bind(&MapLoader::readCallbackMul, this, _1, _2, _3, _4, _5, _6)));
    mulCache_.init(mulStream);
    difCache_.init(difStream);
}

MapLoader::MapLoader(const boost::filesystem::path& mulPath, unsigned int blockCountX, unsigned int blockCountY) :
        blockCountX_(blockCountX), blockCountY_(blockCountY), difEnabled_(false) {

    boost::shared_ptr<FixedSizeOnDemandFileLoader<world::MapBlock> > mulStream(new FixedSizeOnDemandFileLoader<world::MapBlock>(mulPath, 196,
            boost::bind(&MapLoader::readCallbackMul, this, _1, _2, _3, _4, _5, _6)));
    mulCache_.init(mulStream);
}

void MapLoader::readCallbackMul(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<world::MapBlock> item, unsigned int extra, unsigned int userData) {
    unsigned int blockX = userData / 512;
    unsigned int blockY = userData % 512;

    item->blockIndexX_ = blockX;
    item->blockIndexY_ = blockY;

    unsigned int cellXOffset = blockX * 8;
    unsigned int cellYOffset = blockY * 8;

    //LOGARG_INFO(LOGTYPE_DATA, "Loading map blockX=%u blockY=%u cellXOffset=%u cellYOffset=%u", blockX, blockY, cellXOffset, cellYOffset);

    uint16_t artId;
    int8_t z;

    // jump header
    buf += 4;

    // used to set the surrounding z values
    int8_t zValues[10][10];
    memset(&zValues, 0, 100);

    for (unsigned int cellY = 0; cellY < 8; ++cellY) {
        for (unsigned int cellX = 0; cellX < 8; ++cellX) {
            artId = *(reinterpret_cast<uint16_t*>(buf));
            z = *(buf + 2);
            buf += 3;

            item->tiles_[(cellY * 8) + cellX].set(cellX + cellXOffset, cellY + cellYOffset, z, artId);

            zValues[cellY + 1][cellX + 1] = z;
        }
    }

}

void MapLoader::readCallbackDifOffsets(int8_t* buf, unsigned int len) {
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

    if (difEnabled_) {
        // check if there is a dif entry for this block
        std::map<unsigned int, unsigned int>::const_iterator iter = difEntries_.find(idx);
        if (iter != difEntries_.end()) {
            return difCache_.get(iter->second, idx);
        } else {
            return mulCache_.get(idx, idx);
        }
    } else {
        return mulCache_.get(idx, idx);
    }
}

}
}
