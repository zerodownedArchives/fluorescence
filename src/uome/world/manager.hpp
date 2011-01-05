#ifndef UOME_WORLD_MANAGER_HPP
#define UOME_WORLD_MANAGER_HPP

#include <boost/program_options/variables_map.hpp>

namespace uome {
namespace world {

class SectorManager;

class Manager {
public:
    ~Manager();
    static bool create(const boost::program_options::variables_map& config);
    static void destroy();
    static Manager* getSingleton();

    SectorManager* getSectorManager();

private:
    static Manager* singleton_;
    Manager(const boost::program_options::variables_map& config);
    Manager(const Manager& copy) {}
    void operator=(const Manager& copy) {}

    boost::shared_ptr<SectorManager> sectorManager_;
};

}
}

#endif
