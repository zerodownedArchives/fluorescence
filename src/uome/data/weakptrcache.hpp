#ifndef UOME_DATA_WEAKPTRCACHE_HPP
#define UOME_DATA_WEAKPTRCACHE_HPP

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <map>
#include <list>
#include <stdio.h>

#include <logger.hpp>

namespace uome {
namespace data {

/**
 * \brief This class provides caching based on weak pointers. Additionally, it provides support for fixing items in the memory
 */
template <
typename ValueType,
template <typename> class FileLoader
>
class WeakPtrCache {
private:
    typedef std::map<unsigned int, boost::weak_ptr<ValueType> > MapType;
    typedef std::list<boost::shared_ptr<ValueType> > FixedListType;

public:
    WeakPtrCache() {
    }

    void init(boost::shared_ptr<FileLoader<ValueType> > loader) {
        loader_ = loader;
    }

    /// Fixes an item in the memory
    void fixItem(unsigned int id) {
        boost::shared_ptr<ValueType> toFix = get(id);
        fixedList_.push_back(toFix);
    }

    /// Fixes an item in the memory
    void fixItem(boost::shared_ptr<ValueType>& item) {
        fixedList_.push_back(item);
    }

    /// Removes the memory fixation for a given id
    void removeFixed(unsigned int id) {
        boost::shared_ptr<ValueType> toRemove = get(id);
        fixedList_.remove(toRemove);
    }

    /// Removes the memory fixation for a given item
    void removeFixed(boost::shared_ptr<ValueType>& item) {
        fixedList_.remove(item);
    }

    boost::shared_ptr<ValueType> get(unsigned int id) {
        typename MapType::iterator iter = this->cache_.find(id);
        if (iter != this->cache_.end()) {
            // was loaded at some point - is it still valid?
            boost::shared_ptr<ValueType> smPtr = iter->second.lock();
            if (smPtr.get() != NULL) {
                //printf("in cache %u\n", id);
                return smPtr;
            } else {
                // we need to reload it
                //printf("need to reload %u\n", id);
                return this->load(id);
            }
        } else {
            // we need to load it for the first time
            //printf("need to load first time %u\n", id);
            return this->load(id);
        }
    }

private:
    MapType cache_;
    FixedListType fixedList_; ///< Keeps shared_ptr to fixed items. May contain duplicates

    boost::shared_ptr<FileLoader<ValueType> > loader_;

    boost::shared_ptr<ValueType> load(unsigned int id) {
        boost::shared_ptr<ValueType> smPtr = loader_->get(id);
        cache_[id] = boost::weak_ptr<ValueType>(smPtr);
        return smPtr;
    }
};

}
}


#endif
