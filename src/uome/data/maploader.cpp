
#include "maploader.hpp"
#include "fullfileloader.hpp"

#include <world/map.hpp>

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
    unsigned int blockX = userData / blockCountY_;
    unsigned int blockY = userData % blockCountY_;

    item->blockIndexX_ = blockX;
    item->blockIndexY_ = blockY;

    unsigned int cellXOffset = blockX * 8;
    unsigned int cellYOffset = blockY * 8;

    //LOGARG_INFO(LOGTYPE_DATA, "Loading map blockX=%u blockY=%u cellXOffset=%u cellYOffset=%u", blockX, blockY, cellXOffset, cellYOffset);

    uint16_t artId;
    int8_t z;

    // jump header
    buf += 4;

    for (unsigned int cellY = 0; cellY < 8; ++cellY) {
        for (unsigned int cellX = 0; cellX < 8; ++cellX) {
            artId = *(reinterpret_cast<uint16_t*>(buf));
            z = *(buf + 2);
            buf += 3;

            item->get(cellX, cellY)->set(cellX + cellXOffset, cellY + cellYOffset, z, artId);
        }
    }

    setSurroundingZ(item);

}

void MapLoader::setSurroundingZ(boost::shared_ptr<world::MapBlock> item) {
    int8_t zValues[11][11];
    memset(&zValues, 0, 121);

    boost::shared_ptr<world::MapBlock> blocks[3][3];
    bool blocksLoaded[3][3];

    // the current block
    blocks[1][1] = item;
    blocksLoaded[1][1] = true;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) {
                continue;
            }

            int idxX = (int)item->blockIndexX_ + i;
            int idxY = (int)item->blockIndexY_ + j;

            if (idxX < 0 || idxX >= (int)blockCountX_ || idxY < 0 || idxY >= (int)blockCountY_) {
                blocksLoaded[i+1][j+1] = false;
                continue;
            }

            blocks[i+1][j+1] = getNoCreate(idxX, idxY);
            blocksLoaded[i+1][j+1] = blocks[i+1][j+1].get() != NULL && blocks[i+1][j+1]->isReadComplete();
        }
    }

    //LOGARG_DEBUG(LOGTYPE_DATA, "blocks loaded: \n [%i %i %i\n %i %i %i\n %i %i %i]\n",
        //blocksLoaded[0][0], blocksLoaded[0][1], blocksLoaded[0][2],
        //blocksLoaded[1][0], blocksLoaded[1][1], blocksLoaded[1][2],
        //blocksLoaded[2][0], blocksLoaded[2][1], blocksLoaded[2][2]);

    // step 1: fill the z map with the neighboring z values
    unsigned int blockIndices[] = { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2 };
    unsigned int tileIndices[] = { 6, 7, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1 };

    for (unsigned int x = 0; x < 11; ++x) {
        for (unsigned int y = 0; y < 11; ++y) {
            if (blocksLoaded[blockIndices[x]][blockIndices[y]]) {
                zValues[x][y] = blocks[blockIndices[x]][blockIndices[y]]->get(tileIndices[x], tileIndices[y])->getLocZ();
            }
        }
    }

    /*  _____ _____
     * | cur | rig |
     * |_____|_____|
     * | lef | bot |
     * |_____|_____|
     *
     */
    // set the surrounding z values for tiles with a new important neighboring tile
    for (unsigned int x = 1; x < 10; ++x) {
        for (unsigned int y = 1; y < 10; ++y) {
            if (blocksLoaded[blockIndices[x]][blockIndices[y]]) {
                blocks[blockIndices[x]][blockIndices[y]]->get(tileIndices[x], tileIndices[y])->setSurroundingZ(zValues[x][y + 1], zValues[x+1][y], zValues[x + 1][y+1]);
            }
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

boost::shared_ptr<world::MapBlock> MapLoader::getNoCreate(unsigned int x, unsigned int y) {
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
            return difCache_.getNoCreate(iter->second);
        } else {
            return mulCache_.getNoCreate(idx);
        }
    } else {
        return mulCache_.getNoCreate(idx);
    }
}

unsigned int MapLoader::getBlockCountX() {
    return blockCountX_;
}

unsigned int MapLoader::getBlockCountY() {
    return blockCountY_;
}


}
}
