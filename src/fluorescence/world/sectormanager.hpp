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


#ifndef FLUO_DATA_SECTORMANAGER_HPP
#define FLUO_DATA_SECTORMANAGER_HPP

#include <map>
#include <list>

#include <boost/shared_ptr.hpp>

#include <typedefs.hpp>
#include <misc/config.hpp>

namespace fluo {

namespace ui {
namespace components {
class WorldView;
}
}

namespace world {

class Sector;

class SectorManager {
public:
    SectorManager(const Config& config);

    ~SectorManager();

    void registerWorldView(ui::components::WorldView* view);
    void unregisterWorldView(ui::components::WorldView* view);

    /**
     * \param force If true, sector check is forced. If not, sector check only if update frequency kicks in
     */
    void addNewSectors(bool force = false);

    void deleteSectors();

    void clear();

    void update(unsigned int elapsedMillis);
    
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator begin();
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator end();
    
    world::Sector* getSectorForCoordinates(unsigned int locX, unsigned int locY) const;

private:
    std::map<IsoIndex, boost::shared_ptr<world::Sector> > sectorMap_;

    unsigned int updateCounter_; ///< Sector update is not necessary at every frame. Thus we only call it every updateFrequency_ frame
    unsigned int sectorAddFrequency_;

    unsigned int sectorAddDistanceCache_; ///< This many sectors further away from what an ingameview really needs are added
    unsigned int sectorRemoveDistanceCache_; ///< Similar to sectorAddDistanceCache_, but for removal. Should be > sectorAddDistanceCache_

    unsigned int lastMapId_; ///< to detect map changes

    unsigned int calcSectorIndex(unsigned int x, unsigned int y);

    std::list<ui::components::WorldView*> worldViews_;

    void buildSectorRequiredList(std::list<IsoIndex>& list, unsigned int cacheAdd);
};

}
}

#endif
