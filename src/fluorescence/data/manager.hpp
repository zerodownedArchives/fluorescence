#ifndef FLUO_DATA_MANAGER_HPP
#define FLUO_DATA_MANAGER_HPP

#include <misc/exception.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>

#include <ui/textureprovider.hpp>
#include <ui/animation.hpp>

#include <misc/config.hpp>
#include "defstructs.hpp"

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

template<typename ValueType>
class DefFileLoader;

class Manager {
public:
    static bool create(Config& config);
    static void destroy();
    static Manager* getSingleton();
    ~Manager();

    static PaperdollDef getPaperdollDef(unsigned int bodyId);
    static BodyDef getBodyDef(unsigned int baseBodyId);
    static GumpDef getGumpDef(unsigned int gumpId);
    static unsigned int getGumpIdForItem(unsigned int itemId, unsigned int parentBodyId);

    static boost::shared_ptr<ui::TextureProvider> getItemTextureProvider(unsigned int artId);
    static std::vector<boost::shared_ptr<ui::Animation> > getAnim(unsigned int bodyId, unsigned int animId);

    static boost::shared_ptr<ArtLoader> getArtLoader() { return getSingleton()->artLoader_; }
    static boost::shared_ptr<TileDataLoader> getTileDataLoader() { return getSingleton()->tileDataLoader_; }
    static boost::shared_ptr<HuesLoader> getHuesLoader() { return getSingleton()->huesLoader_; }
    static boost::shared_ptr<GumpArtLoader> getGumpArtLoader() { return getSingleton()->gumpArtLoader_; }
    static boost::shared_ptr<MapLoader> getMapLoader(unsigned int index);
    static boost::shared_ptr<StaticsLoader> getStaticsLoader(unsigned int index);
    static boost::shared_ptr<MapTexLoader> getMapTexLoader() { return getSingleton()->mapTexLoader_; }
    static boost::shared_ptr<AnimDataLoader> getAnimDataLoader() { return getSingleton()->animDataLoader_; }
    static boost::shared_ptr<AnimLoader> getAnimLoader(unsigned int index);
    static boost::shared_ptr<MobTypesLoader> getMobTypesLoader() { return getSingleton()->mobTypesLoader_; }
    static boost::shared_ptr<UniFontLoader> getUniFontLoader(unsigned int index);

private:
    static Manager* singleton_;

    Manager();
    void init(Config& config);
    Manager(const Manager& copy) { }
    Manager& operator=(const Manager& copy) { return *this; }

    std::map<std::string, boost::filesystem::path> filePathMap_;
    void buildFilePathMap(Config& config);
    void addToFilePathMap(const boost::filesystem::path& directory);
    bool hasPathFor(const std::string& file) const;
    void checkFileExists(const std::string& file) const;

    boost::shared_ptr<ArtLoader> artLoader_;
    boost::shared_ptr<TileDataLoader> tileDataLoader_;
    boost::shared_ptr<HuesLoader> huesLoader_;
    boost::shared_ptr<GumpArtLoader> gumpArtLoader_;
    boost::shared_ptr<MapTexLoader> mapTexLoader_;
    boost::shared_ptr<AnimDataLoader> animDataLoader_;

    boost::shared_ptr<MapLoader> mapLoader_[6];
    boost::shared_ptr<MapLoader> fallbackMapLoader_;

    boost::shared_ptr<StaticsLoader> staticsLoader_[6];
    boost::shared_ptr<StaticsLoader> fallbackStaticsLoader_;

    boost::shared_ptr<AnimLoader> animLoader_[5];
    boost::shared_ptr<AnimLoader> fallbackAnimLoader_;

    boost::shared_ptr<UniFontLoader> uniFontLoader_[13];
    boost::shared_ptr<UniFontLoader> fallbackUniFontLoader_;

    boost::shared_ptr<MobTypesLoader> mobTypesLoader_;

    boost::shared_ptr<DefFileLoader<BodyDef> > bodyDefLoader_;
    boost::shared_ptr<DefFileLoader<BodyConvDef> > bodyConvDefLoader_;
    boost::shared_ptr<DefFileLoader<PaperdollDef> > paperdollDefLoader_;
    boost::shared_ptr<DefFileLoader<GumpDef> > gumpDefLoader_;
};

}
}

#endif
