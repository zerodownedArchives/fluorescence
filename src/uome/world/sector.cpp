
#include "sector.hpp"

#include <data/manager.hpp>
#include <data/staticsloader.hpp>
#include <data/maploader.hpp>

#include <ui/manager.hpp>
#include <ui/renderqueue.hpp>

namespace uome {
namespace world {

Sector::Sector(unsigned int mapId, unsigned int id, unsigned int locX, unsigned int locY) :
        mapId_(mapId), id_(id), location_(locX, locY), visible_(true) {
    LOGARG_DEBUG(LOGTYPE_WORLD, "Sector construct, map=%u x=%u y=%u", mapId_, location_[0u], location_[1u]);

    mapBlock_ = data::Manager::getMapLoader(mapId_)->get(locX, locY);
    staticBlock_ = data::Manager::getStaticsLoader(mapId_)->get(locX, locY);
}

Sector::~Sector() {
    LOGARG_DEBUG(LOGTYPE_WORLD, "Sector destroy, map=%u x=%u y=%u items=%u", mapId_, location_[0u], location_[1u], staticBlock_->getItemList().size() + 64);
    ui::Manager::getSingleton()->getRenderQueue()->removeSector(this);
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

}
}
