#ifndef FLUO_DATA_INDEXEDONDEMANDFILELOADER_HPP
#define FLUO_DATA_INDEXEDONDEMANDFILELOADER_HPP

#include "indexloader.hpp"
#include "ondemandfileloader.hpp"

namespace fluo {
namespace data {

template <
typename ValueType
>
class IndexedOnDemandFileLoader {

public:
    IndexedOnDemandFileLoader(const boost::filesystem::path& indexPath, const boost::filesystem::path& dataPath,
            typename OnDemandFileLoader<ValueType>::ReadCallback readCallback, unsigned int defaultId = 0) :
            indexLoader_(indexPath), dataLoader_(dataPath, readCallback), defaultId_(defaultId) {
    }

    boost::shared_ptr<ValueType> get(unsigned int index, unsigned int userData) {
        const IndexBlock indexBlock = indexLoader_.get(index);

        // e.g. static blocks containing no data use an offset of 0xFFFFFFFFu
        if (indexBlock.offset_ == 0xFFFFFFFFu || indexBlock.length_ == 0xFFFFFFFFu) {
            //LOG_WARN << "Trying to read nonexistant index entry " << index << std::endl;
            return dataLoader_.get(defaultId_, indexLoader_.get(defaultId_), userData);
         } else {
             return dataLoader_.get(index, indexBlock, userData);
        }
    }

private:
    IndexLoader indexLoader_;
    OnDemandFileLoader<ValueType> dataLoader_;

    unsigned int defaultId_;
};

}
}


#endif
