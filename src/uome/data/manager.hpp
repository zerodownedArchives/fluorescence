#ifndef UOME_DATA_MANAGER_HPP
#define UOME_DATA_MANAGER_HPP


#include "artloader.hpp"
//#include "maploader.hpp"


namespace uome {
namespace data {

class Manager {
public:
    static Manager* getSingleton();
    ~Manager();


    static ArtLoader* getArtLoader() { return getSingleton()->artLoader_.get(); }

private:
    static Manager* singleton_;

    Manager();
    Manager(const Manager& copy) { }
    Manager& operator=(const Manager& copy) { return *this; }


    boost::shared_ptr<ArtLoader> artLoader_;
    //boost::shared_ptr<MapLoader> mapLoader_;
};

}
}

#endif
