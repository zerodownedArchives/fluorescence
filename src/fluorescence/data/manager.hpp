#ifndef FLUO_DATA_MANAGER_HPP
#define FLUO_DATA_MANAGER_HPP

#include <misc/exception.hpp>

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>

#include <ui/textureprovider.hpp>
#include <ui/animation.hpp>

#include <misc/config.hpp>

namespace fluo {
namespace data {

class TileDataLoader;
class ArtLoader;
class HuesLoader;
class GumpArtLoader;
class MapLoader;
class StaticsLoader;
class MapTexLoader;
class AnimDataLoader;
class AnimLoader;
class MobTypesLoader;
class UniFontLoader;

class Manager {
public:
    static bool create(Config& config);
    static void destroy();
    static Manager* getSingleton();
    ~Manager();


    static boost::shared_ptr<ui::TextureProvider> getItemTextureProvider(unsigned int artId);
    static std::vector<boost::shared_ptr<ui::Animation> > getAnim(unsigned int bodyId, unsigned int animId);

    static boost::shared_ptr<ui::Texture> getUnicodeText(unsigned int font, const UnicodeString& text, bool border, unsigned int maxWidth, uint32_t color);

    static ArtLoader* getArtLoader() { return getSingleton()->artLoader_.get(); }
    static TileDataLoader* getTileDataLoader() { return getSingleton()->tileDataLoader_.get(); }
    static HuesLoader* getHuesLoader() { return getSingleton()->huesLoader_.get(); }
    static GumpArtLoader* getGumpArtLoader() { return getSingleton()->gumpArtLoader_.get(); }
    static MapLoader* getMapLoader(unsigned int index);
    static StaticsLoader* getStaticsLoader(unsigned int index);
    static MapTexLoader* getMapTexLoader() { return getSingleton()->mapTexLoader_.get(); }
    static AnimDataLoader* getAnimDataLoader() { return getSingleton()->animDataLoader_.get(); }
    static AnimLoader* getAnimLoader(unsigned int index);
    static MobTypesLoader* getMobTypesLoader() { return getSingleton()->mobTypesLoader_.get(); }

private:
    static Manager* singleton_;

    Manager();
    void init(Config& config);
    Manager(const Manager& copy) { }
    Manager& operator=(const Manager& copy) { return *this; }

    boost::filesystem::path getPathFor(Config& config, const char* configValue);

    boost::shared_ptr<ArtLoader> artLoader_;
    boost::shared_ptr<TileDataLoader> tileDataLoader_;
    boost::shared_ptr<HuesLoader> huesLoader_;
    boost::shared_ptr<GumpArtLoader> gumpArtLoader_;
    boost::shared_ptr<MapTexLoader> mapTexLoader_;
    boost::shared_ptr<AnimDataLoader> animDataLoader_;

    boost::shared_ptr<MapLoader> mapLoader_[5];
    boost::shared_ptr<MapLoader> fallbackMapLoader_;

    boost::shared_ptr<StaticsLoader> staticsLoader_[5];
    boost::shared_ptr<StaticsLoader> fallbackStaticsLoader_;

    boost::shared_ptr<AnimLoader> animLoader_[5];
    boost::shared_ptr<AnimLoader> fallbackAnimLoader_;

    boost::shared_ptr<UniFontLoader> uniFontLoader_[13];

    boost::shared_ptr<MobTypesLoader> mobTypesLoader_;
};

}
}

#endif
