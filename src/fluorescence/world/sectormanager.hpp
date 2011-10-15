#ifndef FLUO_DATA_SECTORMANAGER_HPP
#define FLUO_DATA_SECTORMANAGER_HPP

#include <map>
#include <list>

#include <boost/shared_ptr.hpp>

#include <misc/config.hpp>

namespace fluo {

namespace ui {
    class WorldView;
}

namespace world {

class Sector;

class SectorManager {
public:
    SectorManager(const Config& config);

    ~SectorManager();

    void registerWorldView(ui::WorldView* view);
    void unregisterWorldView(ui::WorldView* view);

    /**
     * \param force If true, sector check is forced. If not, sector check only if update frequency kicks in
     */
    void addNewSectors(bool force = false);

    void deleteSectors();

    void clear();

    void update(unsigned int elapsedMillis);

private:
    std::map<unsigned int, boost::shared_ptr<world::Sector> > sectorMap_;

    unsigned int updateCounter_; ///< Sector update is not necessary at every frame. Thus we only call it every updateFrequency_ frame
    unsigned int sectorAddFrequency_;

    unsigned int sectorAddDistanceCache_; ///< This many sectors further away from what an ingameview really needs are added
    unsigned int sectorRemoveDistanceCache_; ///< Similar to sectorAddDistanceCache_, but for removal. Should be > sectorAddDistanceCache_

    unsigned int lastMapId_; ///< to detect map changes

    unsigned int calcSectorIndex(unsigned int x, unsigned int y);

    std::list<ui::WorldView*> worldViews_;

    void buildSectorRequiredList(std::list<unsigned int>& list, unsigned int cacheAdd);
};

}
}

#endif
