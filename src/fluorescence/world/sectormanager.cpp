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



#include "sectormanager.hpp"

#include <boost/bind.hpp>

#include "manager.hpp"
#include "sector.hpp"
#include "minimapblock.hpp"

#include <misc/log.hpp>

#include <ui/manager.hpp>
#include <ui/components/sectorview.hpp>

#include <data/maploader.hpp>
#include <data/manager.hpp>

namespace fluo {
namespace world {

SectorManager::SectorManager(const Config& config) :
        sectorAddDistanceCache_(2), sectorRemoveDistanceCache_(3) {
}

SectorManager::~SectorManager() {
}

void SectorManager::registerSectorView(ui::components::SectorView* view) {
    sectorViews_.push_back(view);
}

void SectorManager::unregisterSectorView(ui::components::SectorView* view) {
    sectorViews_.remove(view);
}

void SectorManager::onMapChange() {
    clear();
}

void SectorManager::updateSectorList() {
    if (sectorViews_.empty()) {
        return;
    }

    //LOGARG_INFO(LOGTYPE_WORLD, "Sector manager has %u cached", sectorMap_.size());

    unsigned int mapId = world::Manager::getSingleton()->getCurrentMapId();

    // these are all sectors we require for rendering
    std::set<IsoIndex> sectorsFullLoad;
    std::set<IsoIndex> sectorsMiniMap;
    buildSectorRequiredList(sectorsFullLoad, sectorsMiniMap, sectorAddDistanceCache_, mapId);

    // iterate over all stored sectors and remove the ones we do not need anymore
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator iter = sectorMap_.begin();
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator end = sectorMap_.end();
    for(; iter != end; ) {
        if (sectorsFullLoad.count(iter->first) > 0 || sectorsMiniMap.count(iter->first)) {
            ++iter;
        } else {
            sectorMap_.erase(iter++);
        }
    }

    // check all sectors if they are loaded, and load if they are not
    // first for sectors that need to be fully loaded, then sectors for the minimap
    std::map<IsoIndex, boost::shared_ptr<Sector> >::iterator sectorFound;
    std::map<IsoIndex, boost::shared_ptr<Sector> >::const_iterator notFound = sectorMap_.end();
    std::set<IsoIndex>::const_iterator requiredIter = sectorsFullLoad.begin();
    std::set<IsoIndex>::const_iterator requiredEnd = sectorsFullLoad.end();

    for (; requiredIter != requiredEnd; ++requiredIter) {
        sectorFound = sectorMap_.find(*requiredIter);
        if (sectorFound == notFound) {
            boost::shared_ptr<Sector> newSec(new Sector(mapId, *requiredIter, true));
            sectorMap_[*requiredIter] = newSec;
        } else {
            sectorFound->second->setRequireFullLoad(true);
        }
    }

    requiredIter = sectorsMiniMap.begin();
    requiredEnd = sectorsMiniMap.end();

    for (; requiredIter != requiredEnd; ++requiredIter) {
        sectorFound = sectorMap_.find(*requiredIter);
        if (sectorFound == notFound) {
            boost::shared_ptr<Sector> newSec(new Sector(mapId, *requiredIter, false));
            sectorMap_[*requiredIter] = newSec;
        } else {
            sectorFound->second->setRequireFullLoad(false);
        }
    }
}

void SectorManager::clear() {
    sectorMap_.clear();
}

unsigned int SectorManager::calcSectorIndex(unsigned int x, unsigned int y) {
    unsigned int mapId = world::Manager::getSingleton()->getCurrentMapId();
    unsigned int mapHeight = data::Manager::getMapLoader(mapId)->getBlockCountY();
    return x * mapHeight + y;
}

void SectorManager::buildSectorRequiredList(std::set<IsoIndex>& setFull, std::set<IsoIndex>& setMiniMap, unsigned int cacheAdd, unsigned int mapId) {
    // ask all ingame views which sectors they need
    std::list<ui::components::SectorView*>::iterator viewIter = sectorViews_.begin();
    std::list<ui::components::SectorView*>::iterator viewEnd = sectorViews_.end();

    unsigned int mapHeight = data::Manager::getMapLoader(mapId)->getBlockCountY();

    std::set<IsoIndex> tmpMiniMap;

    while (viewIter != viewEnd) {
        ui::components::SectorView* curView = (*viewIter);
        if (curView->requireFullSectorLoad()) {
            curView->getRequiredSectors(setFull, mapHeight, cacheAdd);
        } else {
            curView->getRequiredSectors(tmpMiniMap, mapHeight, cacheAdd);
        }
        ++viewIter;
    }

    // remove elements in miniMapSet which are already a part of the full load set
    std::set_difference(tmpMiniMap.begin(), tmpMiniMap.end(), setFull.begin(), setFull.end(), std::inserter(setMiniMap, setMiniMap.end()));
}

void SectorManager::update(unsigned int elapsedMillis) {
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator iter = sectorMap_.begin();
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator end = sectorMap_.end();

    for (; iter != end; ++iter) {
        iter->second->update(elapsedMillis);
    }
}

std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator SectorManager::begin() {
    return sectorMap_.begin();
}

std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator SectorManager::end() {
    return sectorMap_.end();
}

boost::shared_ptr<world::Sector> SectorManager::getSectorForCoordinates(unsigned int locX, unsigned int locY) {
    locX /= 8;
    locY /= 8;

    IsoIndex secIdx(locX, locY);
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::const_iterator iter = sectorMap_.find(secIdx);

    if (iter != sectorMap_.end()) {
        return iter->second;
    } else {
        boost::shared_ptr<Sector> newSec(new Sector(world::Manager::getSingleton()->getCurrentMapId(), secIdx, false));
        sectorMap_[secIdx] = newSec;
        return newSec;
    }
}

boost::shared_ptr<world::IngameObject> SectorManager::getFirstObjectAt(int worldX, int worldY, bool getTopObject) const {
    boost::shared_ptr<world::IngameObject> ret;

    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::const_reverse_iterator iter = sectorMap_.rbegin();
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::const_reverse_iterator end = sectorMap_.rend();

    for (; iter != end; ++iter) {
        ret = iter->second->getFirstObjectAt(worldX, worldY, getTopObject);

        if (ret) {
            break;
        }
    }

    return ret;
}

void SectorManager::invalidateAllTextures() {
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator iter = sectorMap_.begin();
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator end = sectorMap_.end();

    for (; iter != end; ++iter) {
        iter->second->invalidateAllTextures();
    }
}

boost::shared_ptr<world::MiniMapBlock> SectorManager::getMiniMapBlock(const IsoIndex& idx) {
    IsoIndex miniIdx(idx.x_ - (idx.x_ % MiniMapBlock::SECTOR_ID_MODULO), idx.y_ - (idx.y_ % MiniMapBlock::SECTOR_ID_MODULO));
    std::map<IsoIndex, boost::weak_ptr<world::MiniMapBlock> >::iterator iter = miniMapBlockMap_.find(miniIdx);

    boost::shared_ptr<world::MiniMapBlock> ret;
    if (iter != miniMapBlockMap_.end()) {
        ret = iter->second.lock();
    }

    if (!ret) {
        ret.reset(new MiniMapBlock(miniIdx));
        miniMapBlockMap_[miniIdx] = ret;
    }

    return ret;
}

}
}
