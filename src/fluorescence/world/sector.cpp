
#include "sector.hpp"

#include <data/manager.hpp>
#include <data/staticsloader.hpp>
#include <data/maploader.hpp>

#include <ui/manager.hpp>
#include <ui/render/renderqueue.hpp>

namespace fluo {
namespace world {

Sector::Sector(unsigned int mapId, unsigned int sectorId) :
        mapId_(mapId), id_(sectorId), visible_(true), fullUpdateRenderDataRequired_(true) {

    unsigned int mapHeight = data::Manager::getMapLoader(mapId_)->getBlockCountY();
    location_[0u] = sectorId / mapHeight;
    location_[1u] = sectorId % mapHeight;

    //LOGARG_DEBUG(LOGTYPE_WORLD, "Sector construct, map=%u x=%u y=%u", mapId_, location_[0u], location_[1u]);

    mapBlock_ = data::Manager::getMapLoader(mapId_)->get(location_[0u], location_[1u]);
    staticBlock_ = data::Manager::getStaticsLoader(mapId_)->get(location_[0u], location_[1u]);
}

Sector::~Sector() {
    //LOGARG_DEBUG(LOGTYPE_WORLD, "Sector destroy, map=%u x=%u y=%u items=%u", mapId_, location_[0u], location_[1u], staticBlock_->getItemList().size() + 64);
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

void Sector::removeFromRenderQueue(boost::shared_ptr<ui::RenderQueue> rq) {
    std::list<boost::shared_ptr<world::StaticItem> > staticList = staticBlock_->getItemList();
    std::list<boost::shared_ptr<world::StaticItem> >::const_iterator it = staticList.begin();
    std::list<boost::shared_ptr<world::StaticItem> >::const_iterator end = staticList.end();

    for (; it != end; ++it) {
        (*it)->removeFromRenderQueue(rq);
    }

    for (unsigned int x = 0; x < 8; ++x) {
        for (unsigned int y = 0; y < 8; ++y) {
            mapBlock_->get(x, y)->removeFromRenderQueue(rq);
        }
    }
}

void Sector::update(unsigned int elapsedMillis) {
    if (!staticBlock_->isReadComplete() || !mapBlock_->isReadComplete()) {
        // not fully loaded yet
        return;
    }

    //LOG_DEBUG << "Sector::update " << id_ << std::endl;
    if (fullUpdateRenderDataRequired_) {
        //LOG_DEBUG << "full update required" << std::endl;
        // the sector is not yet loaded completely (e.g. a graphic is still missing)
        bool curFullUpdateRequired = false;

        std::list<boost::shared_ptr<world::StaticItem> > staticList = staticBlock_->getItemList();
        std::list<boost::shared_ptr<world::StaticItem> >::iterator it = staticList.begin();
        std::list<boost::shared_ptr<world::StaticItem> >::iterator end = staticList.end();

        for (; it != end; ++it) {
            if (!(*it)->getWorldRenderData().renderDataValid()) {
                (*it)->updateRenderData(elapsedMillis);
                curFullUpdateRequired = true;
            }
        }

        for (unsigned int x = 0; x < 8; ++x) {
            for (unsigned int y = 0; y < 8; ++y) {
                if (!mapBlock_->get(x, y)->getWorldRenderData().renderDataValid()) {
                    mapBlock_->get(x, y)->updateRenderData(elapsedMillis);
                    curFullUpdateRequired = true;
                }
            }
        }

        if (!curFullUpdateRequired) {
            //LOG_DEBUG << "full update not required anymore" << std::endl;
            // all the items in this sector are now fully loaded, including texture etc
            // to save time in the future, update only the items that are animated. store those in a list now
            quickRenderUpdateList_.clear();

            std::list<boost::shared_ptr<world::StaticItem> >::iterator it = staticList.begin();
            std::list<boost::shared_ptr<world::StaticItem> >::iterator end = staticList.end();

            for (; it != end; ++it) {
                if ((*it)->requireRenderUpdate()) {
                    quickRenderUpdateList_.push_back(*it);
                }
            }

            fullUpdateRenderDataRequired_ = false;

            //LOG_DEBUG << "Items in quicklist: " << quickRenderUpdateList_.size() << std::endl;
        }

    } else if (!quickRenderUpdateList_.empty()) {
        //LOG_DEBUG << "quick render update, size=" << quickRenderUpdateList_.size() << std::endl;
        // only call update on those few things that really need to be updated (e.g. animated statics)
        std::list<boost::shared_ptr<world::StaticItem> >::iterator iter = quickRenderUpdateList_.begin();
        std::list<boost::shared_ptr<world::StaticItem> >::iterator end = quickRenderUpdateList_.end();

        for (; iter != end; ++iter) {
            (*iter)->updateRenderData(elapsedMillis);
        }
    }
}

}
}
