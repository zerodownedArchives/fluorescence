/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef FLUO_DATA_INDEXEDONDEMANDFILELOADER_HPP
#define FLUO_DATA_INDEXEDONDEMANDFILELOADER_HPP

#include "indexloader.hpp"
#include "ondemandfileloader.hpp"

namespace fluo {
namespace data {

template <
typename KeyType,
typename ValueType
>
class IndexedOnDemandFileLoader {

public:
    IndexedOnDemandFileLoader(const boost::filesystem::path& indexPath, const boost::filesystem::path& dataPath,
            typename OnDemandFileLoader<KeyType, ValueType>::ReadCallback readCallback) :
            indexLoader_(indexPath), dataLoader_(dataPath, readCallback) {
    }

    boost::shared_ptr<ValueType> get(KeyType index, unsigned int userData) {
        const IndexBlock indexBlock = indexLoader_.get(index);

        // e.g. static blocks containing no data use an offset of 0xFFFFFFFFu
        if (indexBlock.offset_ == 0xFFFFFFFFu || indexBlock.length_ == 0xFFFFFFFFu) {
            //LOG_WARN << "Trying to read nonexistant index entry " << index << std::endl;
            boost::shared_ptr<ValueType> empty;
            return empty;
         } else {
             return dataLoader_.get(index, indexBlock, userData);
        }
    }

private:
    IndexLoader indexLoader_;
    OnDemandFileLoader<KeyType, ValueType> dataLoader_;
};

}
}


#endif
