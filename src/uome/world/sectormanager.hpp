#ifndef UOME_DATA_SECTORMANAGER_HPP
#define UOME_DATA_SECTORMANAGER_HPP

#include <map>
#include <list>

#include <boost/shared_ptr.hpp>
#include <boost/program_options/variables_map.hpp>

namespace uome {

namespace ui {
    class IngameView;
}

namespace world {

class Sector;

class SectorManager {
public:
    SectorManager(const boost::program_options::variables_map& config);

    ~SectorManager();

    void registerIngameView(boost::shared_ptr<ui::IngameView> view);

    /**
     * \param force If true, sector check is forced. If not, sector check only if update frequency kicks in
     */
    void addNewSectors(bool force = false);

    void deleteSectors();

    void clear();

private:
    std::map<unsigned int, boost::shared_ptr<world::Sector> > sectorMap_;

    unsigned int updateCounter_; ///< Sector update is not necessary at every frame. Thus we only call it every updateFrequency_ frame
    unsigned int updateFrequency_;

    unsigned int sectorAddDistance_; ///< Sectors closer than this are added
    unsigned int sectorRemoveDistance_; ///< Sectors farther away than this are removed. Should be > sectorAddDistance_

    unsigned int lastMapId_; ///< to detect map changes

    unsigned int calcSectorIndex(unsigned int x, unsigned int y);

    std::list<boost::weak_ptr<ui::IngameView> > ingameViews_;

    void buildSectorRequiredList(std::list<unsigned int>& list);
};

}
}

#endif
