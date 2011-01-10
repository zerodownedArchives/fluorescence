
#include "sector.hpp"

#include <data/manager.hpp>
#include <data/staticsloader.hpp>
#include <data/maploader.hpp>

#include <ui/manager.hpp>
#include <ui/renderqueue.hpp>

namespace uome {
namespace world {

Sector::Sector(unsigned int mapId, unsigned int sectorId) :
        mapId_(mapId), id_(sectorId), visible_(true) {

    unsigned int mapHeight = data::Manager::getMapLoader(mapId_)->getBlockCountY();
    location_[0u] = sectorId / mapHeight;
    location_[1u] = sectorId % mapHeight;

    LOGARG_DEBUG(LOGTYPE_WORLD, "Sector construct, map=%u x=%u y=%u", mapId_, location_[0u], location_[1u]);

    mapBlock_ = data::Manager::getMapLoader(mapId_)->get(location_[0u], location_[1u]);
    staticBlock_ = data::Manager::getStaticsLoader(mapId_)->get(location_[0u], location_[1u]);
}

Sector::~Sector() {
    LOGARG_DEBUG(LOGTYPE_WORLD, "Sector destroy, map=%u x=%u y=%u items=%u", mapId_, location_[0u], location_[1u], staticBlock_->getItemList().size() + 64);
}

unsigned int Sector::getLocX() const {
    return location_[0u];
}
unsigned int Sector::getLocY() const {
    return location_[1u];
}

bool Sector::isVisible() const {
    return visible_;
}

boost::shared_ptr<MapBlock> Sector::getMapBlock() const {
    return mapBlock_;
}
boost::shared_ptr<StaticBlock> Sector::getStaticBlock() const {
    return staticBlock_;
}

unsigned int Sector::getMapId() const {
    return mapId_;
}

unsigned int Sector::getSectorId() const {
    return id_;
}

}
}
