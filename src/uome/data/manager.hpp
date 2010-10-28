#ifndef UOME_DATA_MANAGER_HPP
#define UOME_DATA_MANAGER_HPP

#include <exception.hpp>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>

namespace uome {
namespace data {

class TileDataLoader;
class ArtLoader;
class HuesLoader;
class GumpArtLoader;
class MapLoader;
class StaticsLoader;

class Manager {
public:
    static bool create(const boost::program_options::variables_map& config);
    static Manager* getSingleton();
    ~Manager();


    static ArtLoader* getArtLoader() { return getSingleton()->artLoader_.get(); }
    static TileDataLoader* getTileDataLoader() { return getSingleton()->tileDataLoader_.get(); }
    static HuesLoader* getHuesLoader() { return getSingleton()->huesLoader_.get(); }
    static GumpArtLoader* getGumpArtLoader() { return getSingleton()->gumpArtLoader_.get(); }
    static MapLoader* getMapLoader(unsigned int index);
    static StaticsLoader* getStaticsLoader(unsigned int index);

private:
    static Manager* singleton_;

    Manager();
    void init(const boost::program_options::variables_map& config);
    Manager(const Manager& copy) { }
    Manager& operator=(const Manager& copy) { return *this; }

    boost::filesystem::path getPathFor(const boost::program_options::variables_map& config, const char* configValue);

    boost::shared_ptr<ArtLoader> artLoader_;
    boost::shared_ptr<TileDataLoader> tileDataLoader_;
    boost::shared_ptr<HuesLoader> huesLoader_;
    boost::shared_ptr<GumpArtLoader> gumpArtLoader_;

    boost::shared_ptr<MapLoader> mapLoader_[5];
    boost::shared_ptr<MapLoader> fallbackMapLoader_;

    boost::shared_ptr<StaticsLoader> staticsLoader_[5];
    boost::shared_ptr<StaticsLoader> fallbackStaticsLoader_;
};

}
}

#endif
