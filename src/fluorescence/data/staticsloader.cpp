
#include "staticsloader.hpp"
#include "fullfileloader.hpp"

#include <world/mobile.hpp>

#include <boost/bind.hpp>

namespace fluo {
namespace data {

StaticsLoader::StaticsLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath,
                             const boost::filesystem::path& difOffsetsPath, const boost::filesystem::path& difIdxPath, const boost::filesystem::path& difPath,
                             unsigned int blockCountX, unsigned int blockCountY) :
        blockCountX_(blockCountX), blockCountY_(blockCountY), difEnabled_(true) {


    if (boost::filesystem::exists(difOffsetsPath) && boost::filesystem::exists(difIdxPath) && boost::filesystem::exists(difPath)) {
        FullFileLoader difOffsetsLoader(difOffsetsPath);
        difOffsetsLoader.read(boost::bind(&StaticsLoader::readCallbackDifOffsets, this, _1, _2));

        boost::shared_ptr<IndexedOnDemandFileLoader<world::StaticBlock> > difStream(new IndexedOnDemandFileLoader<world::StaticBlock>(difIdxPath, difPath,
            boost::bind(&StaticsLoader::readCallbackMul, this, _1, _2, _3, _4, _5, _6)));

        difCache_.init(difStream);
    } else {
        LOG_WARN << "Unable to open dif files" << std::endl;
        difEnabled_ = false;
    }



    boost::shared_ptr<IndexedOnDemandFileLoader<world::StaticBlock> > mulStream(new IndexedOnDemandFileLoader<world::StaticBlock>(idxPath, mulPath,
            boost::bind(&StaticsLoader::readCallbackMul, this, _1, _2, _3, _4, _5, _6)));

    mulCache_.init(mulStream);
}

StaticsLoader::StaticsLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath,
                             unsigned int blockCountX, unsigned int blockCountY) :
        blockCountX_(blockCountX), blockCountY_(blockCountY), difEnabled_(true) {

    boost::shared_ptr<IndexedOnDemandFileLoader<world::StaticBlock> > mulStream(new IndexedOnDemandFileLoader<world::StaticBlock>(idxPath, mulPath,
            boost::bind(&StaticsLoader::readCallbackMul, this, _1, _2, _3, _4, _5, _6)));
    mulCache_.init(mulStream);
}

void StaticsLoader::readCallbackMul(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<world::StaticBlock> item, unsigned int extra, unsigned int userData) {
    unsigned int blockX = userData / blockCountY_;
    unsigned int blockY = userData % blockCountY_;

    item->blockIndexX_ = blockX;
    item->blockIndexY_ = blockY;

    unsigned int cellOffsetX = blockX * 8;
    unsigned int cellOffsetY = blockY * 8;

    unsigned int itemCount = len / 7;

    uint16_t artId;
    uint8_t cellX;
    uint8_t cellY;
    int8_t cellZ = 0;
    uint16_t hue;

    unsigned int i;

    for (i = 0; i < itemCount; ++i) {
        boost::shared_ptr<world::StaticItem> cur(new world::StaticItem);

        artId = *(reinterpret_cast<uint16_t*>(buf));
        cellX = *(buf + 2);
        cellY = *(buf + 3);
        cellZ = *(reinterpret_cast<int8_t*>(buf + 4));
        hue = *(reinterpret_cast<uint16_t*>(buf + 5));
        buf += 7;

        cur->indexInBlock_ = i;

        cur->set(cellX + cellOffsetX, cellY + cellOffsetY, cellZ, artId, hue);

        item->itemList_.push_back(cur);
    }
}

void StaticsLoader::readCallbackDifOffsets(int8_t* buf, unsigned int len) {
    unsigned int entryCount = len / 4;
    uint32_t* ptr = reinterpret_cast<uint32_t*>(buf);

    for (unsigned int i = 0; i < entryCount; ++i) {
        unsigned int curEntry = *ptr;
        ptr++;
        difEntries_[curEntry] = i;
    }
}

boost::shared_ptr<world::StaticBlock> StaticsLoader::get(unsigned int x, unsigned int y) {
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
