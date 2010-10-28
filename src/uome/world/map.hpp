#ifndef UOME_WORLD_MAPTILE_HPP
#define UOME_WORLD_MAPTILE_HPP

#include "ingameobject.hpp"

#include <data/ondemandreadable.hpp>
#include <data/tiledataloader.hpp>

namespace uome {

namespace data {
    class MapLoader;
}

namespace world {

class MapTile : public IngameObject {

friend class data::MapLoader;

public:
    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const;

private:
    unsigned int artId_;

    const data::LandTileInfo* tileDataInfo_;

    boost::shared_ptr<ui::Texture> texture_;

    void set(int locX, int locY, int locZ, unsigned int artId);

    virtual void updateVertexCoordinates();
    virtual void updateRenderPriority();
};

class MapBlock : public data::OnDemandReadable {

friend class data::MapLoader;

public:
    MapTile* get(unsigned int x, unsigned int y);

private:
    unsigned int blockIndexX_;
    unsigned int blockIndexY_;

    MapTile tiles_[64];
};


}
}

#endif
