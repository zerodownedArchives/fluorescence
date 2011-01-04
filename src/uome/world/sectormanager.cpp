
#include "sectormanager.hpp"

#include <boost/bind.hpp>

#include "sector.hpp"

#include <misc/logger.hpp>

#include <ui/manager.hpp>
#include <ui/ingamewindow.hpp>

namespace uome {
namespace world {
SectorManager::SectorManager(const boost::program_options::variables_map& config) :
        updateCounter_(0) {
    updateFrequency_ = config["world.sector-check-frequency"].as<unsigned int>();
    sectorAddDistance_ = config["world.sector-add-distance"].as<unsigned int>();
    sectorRemoveDistance_ = config["world.sector-remove-distance"].as<unsigned int>();
}

void SectorManager::update(bool force) {
    updateCounter_++;
    bool check = force || (updateCounter_ % updateFrequency_ == 0);

    if (!check) {
        return;
    }

    //LOG_DEBUG(LOGTYPE_WORLD, "Sector manager update");

    //updateCounter_ = 0;

    unsigned int centerSectorX = ui::Manager::getSingleton()->getIngameWindow()->getCenterTileX() / 8;
    unsigned int centerSectorY = ui::Manager::getSingleton()->getIngameWindow()->getCenterTileY() / 8;

    // sectors between these bounds should be loaded
    unsigned int xStart = std::max(0, (int)centerSectorX - (int)sectorAddDistance_);
    unsigned int xEnd = centerSectorX + sectorAddDistance_;

    unsigned int yStart = std::max(0, (int)centerSectorY - (int)sectorAddDistance_);
    unsigned int yEnd = centerSectorY + sectorAddDistance_;

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

    //if (updateCounter_ == 1) {
        //for (unsigned int l = 200; l < 204; ++l) {
            //for (unsigned int m = 170; m < 180; ++m) {
                //boost::shared_ptr<uome::world::Sector> newSec(new uome::world::Sector(m * 512 + l, m, l));
                //sectorList_.push_back(newSec);
            //}
        //}
    //}

    //if (updateCounter_ == 1000) { // ~ 7 sec
        //for (unsigned int l = 204; l < 207; ++l) {
            //for (unsigned int m = 173; m < 177; ++m) {
                //boost::shared_ptr<uome::world::Sector> newSec(new uome::world::Sector(m * 512 + l, m, l));
                //sectorList_.push_back(newSec);
            //}
        //}
    //}

    //if (updateCounter_ == 1500) { // ~ 7 sec
        //for (unsigned int l = 207; l < 210; ++l) {
            //for (unsigned int m = 170; m < 180; ++m) {
                //boost::shared_ptr<uome::world::Sector> newSec(new uome::world::Sector(m * 512 + l, m, l));
                //sectorList_.push_back(newSec);
            //}
        //}
    //}

    if (updateCounter_ == 2000) {
        //sectorList_.remove_if(boost::bind(&SectorManager::checkSectorRemove, this, _1));
    }
}

bool SectorManager::checkSectorRemove(const boost::shared_ptr<uome::world::Sector>& ptr) {
    return (ptr->getLocX() + ptr->getLocY()) % 3 == 0;
}

}
}
