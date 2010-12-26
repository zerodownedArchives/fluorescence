#ifndef UOME_DATA_MAPTEXLOADER_HPP
#define UOME_DATA_MAPTEXLOADER_HPP

#include "weakptrcache.hpp"
#include "indexedondemandfileloader.hpp"

#include <boost/filesystem.hpp>

namespace uome {

namespace ui {
    class Texture;
}

namespace data {

class MapTexLoader {
public:
    MapTexLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath);


    boost::shared_ptr<ui::Texture> get(unsigned int id);

    void readCallback(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<ui::Texture>, unsigned int extra, unsigned int userData);

private:
    WeakPtrCache<ui::Texture, IndexedOnDemandFileLoader> cache_;
};

}
}

#endif
