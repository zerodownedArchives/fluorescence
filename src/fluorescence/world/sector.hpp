#ifndef FLUO_WORLD_SECTOR_HPP
#define FLUO_WORLD_SECTOR_HPP

#include "map.hpp"
#include "statics.hpp"

#include <boost/shared_ptr.hpp>

namespace fluo {

namespace data {
    class SectorReader;
}

namespace ui {
    class RenderQueue;
}

namespace world {

class Sector {

public:
    Sector(unsigned int mapId, unsigned int sectorId);
    ~Sector();

    unsigned int getSectorId() const;
    unsigned int getMapId() const;

    unsigned int getLocX() const;
    unsigned int getLocY() const;

    bool isVisible() const;

    boost::shared_ptr<MapBlock> getMapBlock() const;
    boost::shared_ptr<StaticBlock> getStaticBlock() const;

    void removeFromRenderQueue(boost::shared_ptr<ui::RenderQueue> rq);

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
