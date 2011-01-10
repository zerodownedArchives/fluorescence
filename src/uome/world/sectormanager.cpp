
#include "sectormanager.hpp"

#include <boost/bind.hpp>

#include "manager.hpp"
#include "sector.hpp"

#include <misc/logger.hpp>

#include <ui/manager.hpp>
#include <ui/renderqueue.hpp>
#include <ui/ingameview.hpp>

#include <data/maploader.hpp>
#include <data/manager.hpp>

namespace uome {
namespace world {
SectorManager::SectorManager(const boost::program_options::variables_map& config) :
        updateCounter_(0) {
    updateFrequency_ = config["world.sector-check-frequency"].as<unsigned int>();
    sectorAddDistance_ = config["world.sector-add-distance"].as<unsigned int>();
    sectorRemoveDistance_ = config["world.sector-remove-distance"].as<unsigned int>();
}

SectorManager::~SectorManager() {
}

void SectorManager::registerIngameView(boost::shared_ptr<ui::IngameView> view) {
    boost::weak_ptr<ui::IngameView> toAdd(view);
    ingameViews_.push_back(toAdd);
}

void SectorManager::addNewSectors(bool force) {
    // the player does not move overly fast, so it is enough to check for new sectors every x frames
    bool doAddSector = (updateCounter_ % updateFrequency_ == 0);
    updateCounter_++;

    if (!doAddSector) {
        return;
    }

    LOGARG_INFO(LOGTYPE_WORLD, "Sector manager has %u cached", sectorMap_.size());

    unsigned int mapId = world::Manager::getSingleton()->getCurrentMapId();
    if (mapId != lastMapId_) {
        LOG_INFO(LOGTYPE_WORLD, "Map change detected, clearing sectors");
        clear();
        lastMapId_ = mapId;
    }

    // these are all sectors we require for rendering
    std::list<unsigned int> sectorRequiredList;
    buildSectorRequiredList(sectorRequiredList);


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
        LOG_INFO(LOGTYPE_WORLD, "Map change detected, clearing sectors");
        clear();
        lastMapId_ = mapId;
        return;
    }

    // these are all sectors we require for rendering
    std::list<unsigned int> sectorRequiredList;
    buildSectorRequiredList(sectorRequiredList);


    // iterate over all stored sectors and remove them
    std::map<unsigned int, boost::shared_ptr<Sector> >::iterator deleteIter = sectorMap_.begin();
    std::map<unsigned int, boost::shared_ptr<Sector> >::iterator deleteEnd = sectorMap_.end();

    std::list<unsigned int>::const_iterator requiredBegin = sectorRequiredList.begin();
    std::list<unsigned int>::const_iterator requiredEnd = sectorRequiredList.end();

    while (deleteIter != deleteEnd) {
        if (deleteIter->second->getMapId() != mapId ||
                !std::binary_search(requiredBegin, requiredEnd, deleteIter->second->getSectorId())) {
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

void SectorManager::buildSectorRequiredList(std::list<unsigned int>& list) {
    // ask all ingame views which sectors they need
    std::list<boost::weak_ptr<ui::IngameView> >::iterator viewIter = ingameViews_.begin();
    std::list<boost::weak_ptr<ui::IngameView> >::iterator viewEnd = ingameViews_.end();
    std::list<boost::weak_ptr<ui::IngameView> >::iterator viewDeleteHelper;

    unsigned int mapHeight = data::Manager::getMapLoader(lastMapId_)->getBlockCountY();

    while (viewIter != viewEnd) {
        boost::shared_ptr<ui::IngameView> curView = (*viewIter).lock();
        if (!curView) { // view was removed/closed
            viewDeleteHelper = viewIter;
            ++viewIter;
            ingameViews_.erase(viewDeleteHelper);
            continue;
        }

        curView->getRequiredSectors(list, mapHeight);
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
