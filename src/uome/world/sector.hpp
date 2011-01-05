#ifndef UOME_WORLD_SECTOR_HPP
#define UOME_WORLD_SECTOR_HPP

#include "map.hpp"
#include "statics.hpp"

#include <boost/shared_ptr.hpp>

namespace uome {

namespace data {
    class SectorReader;
}

namespace world {

// TODO: Handle map
class Sector {

public:
    Sector(unsigned int id, unsigned int locX, unsigned int locY);
    ~Sector();

    unsigned int getLocX() { return location_[0u]; }
    unsigned int getLocY() { return location_[1u]; }

    bool isVisible() { return visible_; }

    boost::shared_ptr<MapBlock> getMapBlock() const { return mapBlock_; }
    boost::shared_ptr<StaticBlock> getStaticBlock() const { return staticBlock_; }

private:
    unsigned int id_;
    CL_Vec2<unsigned int> location_;

    boost::shared_ptr<MapBlock> mapBlock_;
    boost::shared_ptr<StaticBlock> staticBlock_;

    bool visible_;

};

}
}

#endif
