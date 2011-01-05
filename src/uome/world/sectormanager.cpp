
#include "sectormanager.hpp"

#include <boost/bind.hpp>

#include "sector.hpp"

#include <misc/logger.hpp>

#include <ui/manager.hpp>
#include <ui/renderqueue.hpp>
#include <ui/ingamewindow.hpp>

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

void SectorManager::addNewSectors(bool force) {
    // the player does not move overly fast, so it is enough to check for new sectors every x frames
    bool doAddSector = (updateCounter_ % updateFrequency_ == 0);
    updateCounter_++;

    if (!doAddSector) {
        return;
    }

    unsigned int centerSectorX = ui::Manager::getSingleton()->getIngameWindow()->getCenterTileX() / 8;
    unsigned int centerSectorY = ui::Manager::getSingleton()->getIngameWindow()->getCenterTileY() / 8;

    // sectors between these bounds should be loaded
    unsigned int xStart = std::max(0, (int)centerSectorX - (int)sectorAddDistance_);
    unsigned int xEnd = centerSectorX + sectorAddDistance_ + 1;

    unsigned int yStart = std::max(0, (int)centerSectorY - (int)sectorAddDistance_);
    unsigned int yEnd = centerSectorY + sectorAddDistance_ + 1;

    // check all sectors if they are loaded, and load if they are not
    std::map<unsigned int, boost::shared_ptr<Sector> >::const_iterator notFound = sectorMap_.end();
    for (unsigned int xIdx = xStart; xIdx < xEnd; ++xIdx) {
        for (unsigned int yIdx = yStart; yIdx < yEnd; ++yIdx) {
            unsigned int curIdx = xIdx * 512 + yIdx;
            if (sectorMap_.find(curIdx) == notFound) {
                boost::shared_ptr<Sector> newSec(new Sector(curIdx, xIdx, yIdx));
                sectorMap_[curIdx] = newSec;
            }
        }
    }
}

void SectorManager::deleteSectors() {
    unsigned int centerSectorX = ui::Manager::getSingleton()->getIngameWindow()->getCenterTileX() / 8;
    unsigned int centerSectorY = ui::Manager::getSingleton()->getIngameWindow()->getCenterTileY() / 8;

    // sectors not between these bounds should be deleted
    unsigned int xStart = std::max(0, (int)centerSectorX - (int)sectorRemoveDistance_);
    unsigned int xEnd = centerSectorX + sectorRemoveDistance_ + 1;

    unsigned int yStart = std::max(0, (int)centerSectorY - (int)sectorRemoveDistance_);
    unsigned int yEnd = centerSectorY + sectorRemoveDistance_ + 1;

    std::map<unsigned int, boost::shared_ptr<Sector> >::iterator deleteIter = sectorMap_.begin();
    std::map<unsigned int, boost::shared_ptr<Sector> >::iterator deleteEnd = sectorMap_.end();

    while (deleteIter != deleteEnd) {
        if (deleteIter->second->getLocX() < xStart ||
                deleteIter->second->getLocX() > xEnd ||
                deleteIter->second->getLocY() < yStart ||
                deleteIter->second->getLocY() > yEnd) {
            sectorMap_.erase(deleteIter);
            break;
        }

        ++deleteIter;
    }
}


void SectorManager::clear() {
    sectorMap_.clear();
}

}
}
