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


#ifndef FLUO_WORLD_MAPTILE_HPP
#define FLUO_WORLD_MAPTILE_HPP

#include "ingameobject.hpp"

#include <data/ondemandreadable.hpp>
#include <data/tiledataloader.hpp>

namespace fluo {

namespace data {
    class MapLoader;
}

namespace world {
class Sector;

class MapTile : public IngameObject {

friend class data::MapLoader;

public:
    MapTile();
    virtual boost::shared_ptr<ui::Texture> getIngameTexture() const;

    unsigned int getArtId();
    const data::LandTileInfo* getTileDataInfo();

    virtual bool hasPixel(int pixelX, int pixelY) const;
    virtual bool overlaps(const CL_Rectf& rect) const;

    void setVertexNormals(const CL_Vec3f& top, const CL_Vec3f& right, const CL_Vec3f& bottom, const CL_Vec3f& left);

    virtual void onClick();
    
    float getAverageZ() const;
    int getMaxZ() const;

private:
    unsigned int artId_;

    const data::LandTileInfo* tileDataInfo_;

    boost::shared_ptr<ui::Texture> texture_;

    void set(int locX, int locY, int locZ, unsigned int artId);

    int zLeft_;
    int zRight_;
    int zBottom_;
    void setSurroundingZ(int left, int right, int bottom);

    void updateVertexCoordinates();
    void updateRenderDepth();
    void updateTextureProvider();
    bool updateAnimation(unsigned int elapsedMillis);

    void setTexture();

    bool isPixelInside(int pixelX, int pixelY, const CL_Vec2f& b, const CL_Vec2f& c) const;

    void calculateIsFlat();
    bool isFlat_;
    
    static bool isIdIgnored(unsigned int artid);
    static bool isIdWater(unsigned int artid);
};

class MapBlock : public data::OnDemandReadable<MapBlock> {

friend class data::MapLoader;
friend class world::Sector;

public:
    MapBlock();
    boost::shared_ptr<MapTile> get(unsigned int x, unsigned int y);

private:
    unsigned int blockIndexX_;
    unsigned int blockIndexY_;

    boost::shared_ptr<MapTile> tiles_[64];

    bool repaintRequested_; // used when a neighboring tile is loaded to adjust all textures and z levels
};


}
}

#endif
