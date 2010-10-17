#ifndef UOME_DATA_FIXEDSIZEONDEMANDFILELOADER_HPP
#define UOME_DATA_FIXEDSIZEONDEMANDFILELOADER_HPP

#include "ondemandfileloader.hpp"

namespace uome {
namespace data {

template <
typename ValueType
>
class FixedSizeOnDemandFileLoader : public OnDemandFileLoader<ValueType> {

public:
    FixedSizeOnDemandFileLoader(const boost::filesystem::path& path, unsigned int size, typename OnDemandFileLoader<ValueType>::ReadCallback readCallback) throw(Exception) :
            OnDemandFileLoader<ValueType>(path, readCallback), size_(size) {
    }

    boost::shared_ptr<ValueType> get(unsigned int index) {
        unsigned int startOffset = index * size_;
        return this->OnDemandFileLoader<ValueType>::get(startOffset, size_);
    }

private:
    unsigned int size_;

};

}
}


#endif
