#ifndef UOME_DATA_INDEXEDONDEMANDFILELOADER_HPP
#define UOME_DATA_INDEXEDONDEMANDFILELOADER_HPP

#include "indexloader.hpp"
#include "ondemandfileloader.hpp"

namespace uome {
namespace data {

template <
typename ValueType
>
class IndexedOnDemandFileLoader {

public:
    IndexedOnDemandFileLoader(const boost::filesystem::path& indexPath, const boost::filesystem::path& dataPath, typename OnDemandFileLoader<ValueType>::ReadCallback readCallback) throw(Exception) :
            indexLoader_(indexPath), dataLoader_(dataPath, readCallback) {
    }

    boost::shared_ptr<ValueType> get(unsigned int index) {
        const IndexBlock* indexBlock = indexLoader_.get(index);
        return dataLoader_.get(index, indexBlock->offset_, indexBlock->length_);
    }

private:
    IndexLoader indexLoader_;
    OnDemandFileLoader<ValueType> dataLoader_;

};

}
}


#endif
