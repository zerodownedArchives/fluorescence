#ifndef FLUO_DATA_GUMPARTLOADER_HPP
#define FLUO_DATA_GUMPARTLOADER_HPP

#include "weakptrcache.hpp"
#include "indexedondemandfileloader.hpp"

#include <boost/filesystem.hpp>

namespace fluo {

namespace ui {
    class Texture;
}

namespace data {

class GumpArtLoader {
public:
    GumpArtLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath);

    boost::shared_ptr<ui::Texture> getTexture(unsigned int id);

    void readCallback(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<ui::Texture>, unsigned int extra, unsigned int userData);

private:
    WeakPtrCache<ui::Texture, IndexedOnDemandFileLoader> cache_;
};

}
}

#endif
