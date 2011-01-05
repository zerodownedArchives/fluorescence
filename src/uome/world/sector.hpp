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

class Sector {

public:
    Sector(unsigned int mapId, unsigned int id, unsigned int locX, unsigned int locY);
    ~Sector();

    unsigned int getMapId() const;

    unsigned int getLocX() const;
    unsigned int getLocY() const;

    bool isVisible() const;

    boost::shared_ptr<MapBlock> getMapBlock() const;
    boost::shared_ptr<StaticBlock> getStaticBlock() const;

private:
    unsigned int mapId_;
    unsigned int id_;
    CL_Vec2<unsigned int> location_;

    boost::shared_ptr<MapBlock> mapBlock_;
    boost::shared_ptr<StaticBlock> staticBlock_;

    bool visible_;

};

}
}

#endif
