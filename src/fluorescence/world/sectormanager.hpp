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
#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <typedefs.hpp>
#include <misc/config.hpp>

namespace fluo {

namespace ui {
namespace components {
class SectorView;
}
}

namespace world {

class Sector;
class IngameObject;
class MiniMapBlock;

class SectorManager {
public:
    SectorManager(const Config& config);

    ~SectorManager();

    void registerSectorView(ui::components::SectorView* view);
    void unregisterSectorView(ui::components::SectorView* view);

    void updateSectorList();

    void onMapChange();
    void clear();

    void update(unsigned int elapsedMillis);

    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator begin();
    std::map<IsoIndex, boost::shared_ptr<world::Sector> >::iterator end();

    boost::shared_ptr<world::Sector> getSectorForCoordinates(unsigned int locX, unsigned int locY);

    boost::shared_ptr<IngameObject> getFirstObjectAt(int worldX, int worldY, bool getTopObject) const;

    void invalidateAllTextures();

    boost::shared_ptr<world::MiniMapBlock> getMiniMapBlock(const IsoIndex& idx);

private:
    std::map<IsoIndex, boost::shared_ptr<world::Sector> > sectorMap_;

    unsigned int sectorAddDistanceCache_; ///< This many sectors further away from what an ingameview really needs are added
    unsigned int sectorRemoveDistanceCache_; ///< Similar to sectorAddDistanceCache_, but for removal. Should be > sectorAddDistanceCache_

    unsigned int calcSectorIndex(unsigned int x, unsigned int y);

    std::list<ui::components::SectorView*> sectorViews_;

    void buildSectorRequiredList(std::set<IsoIndex>& setFull, std::set<IsoIndex>& setMiniMap, unsigned int cacheAdd, unsigned int mapId);

    std::map<IsoIndex, boost::weak_ptr<world::MiniMapBlock> > miniMapBlockMap_;
};

}
}

#endif
