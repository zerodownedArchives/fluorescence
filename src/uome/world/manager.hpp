#ifndef UOME_WORLD_MANAGER_HPP
#define UOME_WORLD_MANAGER_HPP

#include <boost/program_options/variables_map.hpp>

namespace uome {
namespace world {

class SectorManager;
class LightManager;

class Manager {
public:
    ~Manager();
    static bool create(const boost::program_options::variables_map& config);
    static void destroy();
    static Manager* getSingleton();

    boost::shared_ptr<SectorManager> getSectorManager();

    unsigned int getCurrentMapId();
    void setCurrentMapId(unsigned int id);

    boost::shared_ptr<LightManager> getLightManager();

private:
    static Manager* singleton_;
    Manager(const boost::program_options::variables_map& config);
    Manager(const Manager& copy) {}
    void operator=(const Manager& copy) {}

    boost::shared_ptr<SectorManager> sectorManager_;
    unsigned int currentMapId_;

    boost::shared_ptr<LightManager> lightManager_;
};

}
}

#endif
