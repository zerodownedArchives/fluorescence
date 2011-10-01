#ifndef UOME_WORLD_MANAGER_HPP
#define UOME_WORLD_MANAGER_HPP

#include <boost/shared_ptr.hpp>

#include <misc/config.hpp>

#include "mobile.hpp"
#include "dynamicitem.hpp"

namespace uome {

namespace world {

class SectorManager;
class LightManager;

class Manager {
public:
    ~Manager();
    static bool create(const Config& config);
    static void destroy();
    static Manager* getSingleton();

    boost::shared_ptr<SectorManager> getSectorManager();

    unsigned int getCurrentMapId();
    void setCurrentMapId(unsigned int id);

    boost::shared_ptr<LightManager> getLightManager();

    boost::shared_ptr<Mobile> getPlayer();

    boost::shared_ptr<Mobile> getMobile(Serial serial, bool createIfNotExists = true);
    boost::shared_ptr<DynamicItem> getDynamicItem(Serial serial, bool createIfNotExists = true);

    boost::shared_ptr<Mobile> initPlayer(Serial serial);

    void deleteObject(Serial serial);

    void printItemSerials();

private:
    static Manager* singleton_;
    Manager(const Config& config);
    Manager(const Manager& copy) {}
    void operator=(const Manager& copy) {}

    boost::shared_ptr<SectorManager> sectorManager_;
    unsigned int currentMapId_;

    boost::shared_ptr<LightManager> lightManager_;

    boost::shared_ptr<Mobile> player_;
    std::map<Serial, boost::shared_ptr<Mobile> > mobiles_;

    std::map<Serial, boost::shared_ptr<DynamicItem> > dynamicItems_;
};

}
}

#endif
