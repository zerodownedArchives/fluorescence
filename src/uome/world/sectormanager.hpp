#ifndef UOME_DATA_SECTORMANAGER_HPP
#define UOME_DATA_SECTORMANAGER_HPP

#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/program_options/variables_map.hpp>

namespace uome {
namespace world {

class Sector;

class SectorManager {
public:
    SectorManager(const boost::program_options::variables_map& config);

    /**
     * \param force If true, sector check is forced. If not, sector check only if update frequency kicks in
     */
    void update(bool force = false);

private:
    std::map<unsigned int, boost::shared_ptr<world::Sector> > sectorMap_;

    unsigned int updateCounter_; ///< Sector update is not necessary at every frame. Thus we only call it every updateFrequency_ frame
    unsigned int updateFrequency_;

    bool checkSectorRemove(const boost::shared_ptr<uome::world::Sector>& ptr);

    unsigned int sectorAddDistance_; ///< Sectors closer than this are added
    unsigned int sectorRemoveDistance_; ///< Sectors farther away than this are removed. Should be > sectorAddDistance_
};

}
}

#endif
