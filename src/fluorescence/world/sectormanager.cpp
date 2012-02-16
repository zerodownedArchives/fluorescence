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

#include <misc/log.hpp>

#include <ui/manager.hpp>
#include <ui/render/renderqueue.hpp>
#include <ui/components/worldview.hpp>

#include <data/maploader.hpp>
#include <data/manager.hpp>

namespace fluo {
namespace world {
SectorManager::SectorManager(const Config& config) :
        updateCounter_(0) {
    sectorAddFrequency_ = 60;
    sectorAddDistanceCache_ = 2;
    sectorRemoveDistanceCache_ = 3;
}

SectorManager::~SectorManager() {
}

void SectorManager::registerWorldView(ui::components::WorldView* view) {
    worldViews_.push_back(view);
}

void SectorManager::unregisterWorldView(ui::components::WorldView* view) {
    worldViews_.remove(view);
}

void SectorManager::addNewSectors(bool force) {
    // the player does not move overly fast, so it is enough to check for new sectors every x frames
    bool doAddSector = (updateCounter_ % sectorAddFrequency_ == 0);
    updateCounter_++;

    if (!doAddSector) {
        return;
    }

    //LOGARG_INFO(LOGTYPE_WORLD, "Sector manager has %u cached", sectorMap_.size());

    unsigned int mapId = world::Manager::getSingleton()->getCurrentMapId();
    if (mapId != lastMapId_) {
        LOG_INFO << "SectorManager: map change detected, clearing sectors" << std::endl;
        clear();
        lastMapId_ = mapId;
    }

    // these are all sectors we require for rendering
    std::list<IsoIndex> sectorRequiredList;
    buildSectorRequiredList(sectorRequiredList, sectorAddDistanceCache_);


    std::list<IsoIndex>::const_iterator requiredIter = sectorRequiredList.begin();
    std::list<IsoIndex>::const_iterator requiredEnd = sectorRequiredList.end();

    // check all sectors if they are loaded, and load if they are not
    std::map<IsoIndex, boost::shared_ptr<Sector> >::const_iterator notFound = sectorMap_.end();

    for (; requiredIter != requiredEnd; ++requiredIter) {
        IsoIndex curIdx = *requiredIter;
        if (sectorMap_.find(curIdx) == notFound) {
            boost::shared_ptr<Sector> newSec(new Sector(mapId, curIdx));
            sectorMap_[curIdx] = newSec;
        }
    }
}

void SectorManager::deleteSectors() {
    unsigned int mapId = world::Manager::getSingleton()->getCurrentMapId();

    if (mapId != lastMapId_) {
        LOG_INFO << "Map change detected, clearing sectors" << std::endl;
        lastMapId_ = mapId;
    }

    // these are all sectors we require for rendering
    std::list<IsoIndex> sectorRequiredList;
    buildSectorRequiredList(sectorRequiredList, sectorRemoveDistanceCache_);


    // iterate over all stored sectors and remove them
    std::map<IsoIndex, boost::shared_ptr<Sector> >::iterator deleteIter = sectorMap_.begin();
    std::map<IsoIndex, boost::shared_ptr<Sector> >::iterator deleteEnd = sectorMap_.end();

    std::list<IsoIndex>::const_iterator requiredBegin = sectorRequiredList.begin();
    std::list<IsoIndex>::const_iterator requiredEnd = sectorRequiredList.end();

    boost::shared_ptr<ui::RenderQueue> renderQueue = ui::Manager::getWorldRenderQueue();

    while (deleteIter != deleteEnd) {
        if (deleteIter->second->getMapId() != mapId ||
                !std::binary_search(requiredBegin, requiredEnd, deleteIter->second->getSectorId())) {
            deleteIter->second->removeFromRenderQueue(renderQueue);
            sectorMap_.erase(deleteIter);
        }

        ++deleteIter;
    }
}


void SectorManager::clear() {
    sectorMap_.clear();
}

unsigned int SectorManager::calcSectorIndex(unsigned int x, unsigned int y) {
    unsigned int mapHeight = data::Manager::getMapLoader(lastMapId_)->getBlockCountY();
    return x * mapHeight + y;
}

void SectorManager::buildSectorRequiredList(std::list<IsoIndex>& list, unsigned int cacheAdd) {
    // ask all ingame views which sectors they need
    std::list<ui::components::WorldView*>::iterator viewIter = worldViews_.begin();
    std::list<ui::components::WorldView*>::iterator viewEnd = worldViews_.end();

    unsigned int mapHeight = data::Manager::getMapLoader(lastMapId_)->getBlockCountY();

    while (viewIter != viewEnd) {
        ui::components::WorldView* curView = (*viewIter);
        curView->getRequiredSectors(list, mapHeight, cacheAdd);
        ++viewIter;
    }

    list.sort();

    // remove duplicates
    std::list<IsoIndex>::iterator newListEnd = std::unique(list.begin(), list.end());
    std::list<IsoIndex>::iterator listEnd = list.end();

    if (newListEnd != listEnd) {
        // count, how many elements we need to cut off
        std::list<IsoIndex>::iterator countIter = newListEnd;


        unsigned int cutOffCount = 0;
        for(; countIter != listEnd; ++countIter) {
            ++cutOffCount;
        }

        unsigned int newListLen = list.size() - cutOffCount;
        list.resize(newListLen);
    }
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

world::Sector* SectorManager::getSectorForCoordinates(unsigned int locX, unsigned int locY) const {
    locX /= 8;
    locY /= 8;
    
    IsoIndex secIdx(locX, locY);
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::const_iterator iter = sectorMap_.find(secIdx);
    
    return (iter != sectorMap_.end()) ? iter->second.get() : NULL;
}

}
}
