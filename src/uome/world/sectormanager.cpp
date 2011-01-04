
#include "sectormanager.hpp"

#include <boost/bind.hpp>

#include "sector.hpp"

#include <misc/logger.hpp>

namespace uome {
namespace world {
SectorManager::SectorManager(const boost::program_options::variables_map& config) :
        updateCounter_(0) {
    updateFrequency_ = config["world.sector-check-frequency"].as<unsigned int>();
}

void SectorManager::update(bool force) {
    updateCounter_++;
    bool check = force || (updateCounter_ % updateFrequency_ == 0);

    if (!check) {
        return;
    }

    //LOG_DEBUG(LOGTYPE_WORLD, "Sector manager update");

    //updateCounter_ = 0;

    if (updateCounter_ == 1) {
        for (unsigned int l = 200; l < 204; ++l) {
            for (unsigned int m = 170; m < 180; ++m) {
                boost::shared_ptr<uome::world::Sector> newSec(new uome::world::Sector(m * 512 + l, m, l));
                sectorList_.push_back(newSec);
            }
        }
    }

    if (updateCounter_ == 1000) { // ~ 7 sec
        for (unsigned int l = 204; l < 207; ++l) {
            for (unsigned int m = 173; m < 177; ++m) {
                boost::shared_ptr<uome::world::Sector> newSec(new uome::world::Sector(m * 512 + l, m, l));
                sectorList_.push_back(newSec);
            }
        }
    }

    if (updateCounter_ == 1500) { // ~ 7 sec
        for (unsigned int l = 207; l < 210; ++l) {
            for (unsigned int m = 170; m < 180; ++m) {
                boost::shared_ptr<uome::world::Sector> newSec(new uome::world::Sector(m * 512 + l, m, l));
                sectorList_.push_back(newSec);
            }
        }
    }

    if (updateCounter_ == 2000) {
        sectorList_.remove_if(boost::bind(&SectorManager::checkSectorRemove, this, _1));
    }
}

bool SectorManager::checkSectorRemove(const boost::shared_ptr<uome::world::Sector>& ptr) {
    return (ptr->getLocX() + ptr->getLocY()) % 3 == 0;
}

}
}
