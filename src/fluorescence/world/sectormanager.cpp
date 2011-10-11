
#include "sectormanager.hpp"

#include <boost/bind.hpp>

#include "manager.hpp"
#include "sector.hpp"

#include <misc/log.hpp>

#include <ui/manager.hpp>
#include <ui/ingamerenderqueue.hpp>
#include <ui/components/ingameview.hpp>

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

void SectorManager::registerIngameView(ui::IngameView* view) {
    ingameViews_.push_back(view);
}

void SectorManager::unregisterIngameView(ui::IngameView* view) {
    ingameViews_.remove(view);
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
    std::list<unsigned int> sectorRequiredList;
    buildSectorRequiredList(sectorRequiredList, sectorAddDistanceCache_);


    std::list<unsigned int>::const_iterator requiredIter = sectorRequiredList.begin();
    std::list<unsigned int>::const_iterator requiredEnd = sectorRequiredList.end();

    // check all sectors if they are loaded, and load if they are not
    std::map<unsigned int, boost::shared_ptr<Sector> >::const_iterator notFound = sectorMap_.end();

    for (; requiredIter != requiredEnd; ++requiredIter) {
        unsigned int curIdx = *requiredIter;
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
        clear();
        lastMapId_ = mapId;
        return;
    }

    // these are all sectors we require for rendering
    std::list<unsigned int> sectorRequiredList;
    buildSectorRequiredList(sectorRequiredList, sectorRemoveDistanceCache_);


    // iterate over all stored sectors and remove them
    std::map<unsigned int, boost::shared_ptr<Sector> >::iterator deleteIter = sectorMap_.begin();
    std::map<unsigned int, boost::shared_ptr<Sector> >::iterator deleteEnd = sectorMap_.end();

    std::list<unsigned int>::const_iterator requiredBegin = sectorRequiredList.begin();
    std::list<unsigned int>::const_iterator requiredEnd = sectorRequiredList.end();

    boost::shared_ptr<ui::IngameRenderQueue> renderQueue = ui::Manager::getWorldRenderQueue();

    while (deleteIter != deleteEnd) {
        if (deleteIter->second->getMapId() != mapId ||
                !std::binary_search(requiredBegin, requiredEnd, deleteIter->second->getSectorId())) {
            renderQueue->remove(deleteIter->second);
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

void SectorManager::buildSectorRequiredList(std::list<unsigned int>& list, unsigned int cacheAdd) {
    // ask all ingame views which sectors they need
    std::list<ui::IngameView*>::iterator viewIter = ingameViews_.begin();
    std::list<ui::IngameView*>::iterator viewEnd = ingameViews_.end();

    unsigned int mapHeight = data::Manager::getMapLoader(lastMapId_)->getBlockCountY();

    while (viewIter != viewEnd) {
        ui::IngameView* curView = (*viewIter);
        curView->getRequiredSectors(list, mapHeight, cacheAdd);
        ++viewIter;
    }

    list.sort();

    // remove duplicates
    std::list<unsigned int>::iterator newListEnd = std::unique(list.begin(), list.end());
    std::list<unsigned int>::iterator listEnd = list.end();

    if (newListEnd != listEnd) {
        // count, how many elements we need to cut off
        std::list<unsigned int>::iterator countIter = newListEnd;


        unsigned int cutOffCount = 0;
        for(; countIter != listEnd; ++countIter) {
            ++cutOffCount;
        }

        unsigned int newListLen = list.size() - cutOffCount;
        list.resize(newListLen);
    }
}

}
}
