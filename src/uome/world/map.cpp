
#include "map.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/tiledataloader.hpp>

namespace uome {
namespace world {

boost::shared_ptr<ui::Texture> MapTile::getIngameTexture() const {
    return texture_;
}

void MapTile::set(int locX, int locY, int locZ, unsigned int artId) {
    artId_ = artId;
    tileDataInfo_ = data::Manager::getTileDataLoader()->getLandTileInfo(artId_);
    texture_ = data::Manager::getArtLoader()->getMapTexture(artId_);

    setLocation(locX, locY, locZ);
}

void MapTile::updateVertexCoordinates() {

    int px = (getLocX() - getLocY()) * 22;
    int py = (getLocX() + getLocY()) * 22;
    py -= getLocZ() * 4;

    CL_Rectf rect(px, py, px + 44, py + 44);

    vertexCoordinates_[0] = CL_Vec2f(rect.left, rect.top);
    vertexCoordinates_[1] = CL_Vec2f(rect.right, rect.top);
    vertexCoordinates_[2] = CL_Vec2f(rect.left, rect.bottom);
    vertexCoordinates_[3] = CL_Vec2f(rect.right, rect.top);
    vertexCoordinates_[4] = CL_Vec2f(rect.left, rect.bottom);
    vertexCoordinates_[5] = CL_Vec2f(rect.right, rect.bottom);
}

void MapTile::updateRenderPriority() {
    renderPriority_[0] = getLocX() + getLocY();
    renderPriority_[1] = getLocZ();
}

MapTile* MapBlock::get(unsigned int x, unsigned int y) {
    if (x > 7) {
        x = 0;
    }

    if (y > 7) {
        y = 0;
    }

    return &tiles_[(y*8) + x];
}

}
}
