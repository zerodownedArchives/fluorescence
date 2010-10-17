#ifndef UOME_WORLD_SECTOR_HPP
#define UOME_WORLD_SECTOR_HPP

namespace uome {

namespace data {
    class SectorReader;
}

namespace world {

class Sector {

friend class data::SectorReader;

public:
    Sector(int locX, int locY);

    int getLocX() { return location_[0]; }
    int getLocY() { return location_[1]; }

    bool isVisible() { return visible_; }

private:
    unsigned int id_;
    CL_Vec2<int> location_;

    MapTile mapTiles_[64];

    bool visible_;

    // these 2 variables indicate wheter or not the sector has been fully read from the map and statics mul file
    bool mapLoaded_;
    bool staticsLoaded_;
};

}
}
