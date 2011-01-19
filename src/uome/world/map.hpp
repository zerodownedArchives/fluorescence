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
    MapTile();
    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const;

    unsigned int getArtId();
    const data::LandTileInfo* getTileDataInfo();

    bool isFlat() const;

    virtual bool hasPixel(int pixelX, int pixelY) const;
    virtual bool isInDrawArea(int leftPixelCoord, int rightPixelCoord, int topPixelCoord, int bottomPixelCoord) const;

    void setVertexNormals(const CL_Vec3f& top, const CL_Vec3f& right, const CL_Vec3f& bottom, const CL_Vec3f& left);

    virtual void onClick();

private:
    unsigned int artId_;

    const data::LandTileInfo* tileDataInfo_;

    boost::shared_ptr<ui::Texture> texture_;

    void set(int locX, int locY, int locZ, unsigned int artId);

    int zLeft_;
    int zRight_;
    int zBottom_;
    void setSurroundingZ(int left, int right, int bottom);

    virtual void updateVertexCoordinates();
    virtual void updateRenderPriority();
    virtual void updateTextureProvider();

    void setTexture();

    bool isPixelInside(int pixelX, int pixelY, const CL_Vec2f& b, const CL_Vec2f& c) const;
};

class MapBlock : public data::OnDemandReadable {

friend class data::MapLoader;

public:
    MapBlock();
    boost::shared_ptr<MapTile> get(unsigned int x, unsigned int y);

private:
    unsigned int blockIndexX_;
    unsigned int blockIndexY_;

    boost::shared_ptr<MapTile> tiles_[64];
};


}
}

#endif
