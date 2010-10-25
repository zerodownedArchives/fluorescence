#ifndef UOME_WORLD_MAPTILE_HPP
#define UOME_WORLD_MAPTILE_HPP

#include "ingameobject.hpp"

#include <data/ondemandreadable.hpp>
#include <data/manager.hpp>
#include <data/artloader.hpp>

namespace uome {

namespace data {
    class MapLoader;
}

namespace world {

class MapTile : public IngameObject {

friend class data::MapLoader;

public:
    MapTile() { }

    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const {
        return texture_;
    }

private:
    unsigned int artId_;

    boost::shared_ptr<ui::Texture> texture_;

    void set(int locX, int locY, int locZ, unsigned int artId) {
        artId_ = artId;
        texture_ = data::Manager::getArtLoader()->getMapTexture(artId_);

        setLocation(locX, locY, locZ);
    }
};

class MapBlock : public data::OnDemandReadable {

friend class data::MapLoader;

public:
    const MapTile& get(unsigned int x, unsigned int y) {
        if (x > 7) {
            x = 0;
        }

        if (y > 7) {
            y = 0;
        }

        return tiles_[(y*8) + x];
    }

private:
    unsigned int blockIndexX;
    unsigned int blockIndexY;

    MapTile tiles_[64];
};


}
}

#endif
