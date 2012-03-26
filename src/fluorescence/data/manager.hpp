/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


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
class UniFontLoader;
class EquipConvDefLoader;
class ClilocLoader;
class HttpLoader;
class FilePathLoader;

template<typename ValueType>
class DefFileLoader;

struct TextureSource {
enum {
	FILE = 1,
	MAPART = 2,
	STATICART = 3,
	GUMPART = 4,
    HTTP = 5,
};
};

struct FileFormat {
enum {
    MUL = 1,
    MUL_HIGH_SEAS = 2,
    UOP = 3,
};
};


class Manager {
public:
    static bool create(Config& config);
    static void destroy();
    static Manager* getSingleton();
    ~Manager();
    
    // return the path to the file inside the current shard directory, if the file exists there.
    // if not, return default path
    static boost::filesystem::path getShardFilePath(const boost::filesystem::path& innerPath);

    static PaperdollDef getPaperdollDef(unsigned int bodyId);
    static BodyDef getBodyDef(unsigned int baseBodyId);
    static GumpDef getGumpDef(unsigned int gumpId);
    static unsigned int getGumpIdForItem(unsigned int itemId, unsigned int parentBodyId);
    static MountDef getMountDef(unsigned int itemId);
    static MobTypeDef getMobTypeDef(unsigned int bodyId);
    static EffectTranslationDef getEffectTranslationDef(unsigned int effectId);
    static MusicConfigDef getMusicConfigDef(unsigned int musicId);

    static boost::shared_ptr<ui::TextureProvider> getItemTextureProvider(unsigned int artId);
    static std::vector<boost::shared_ptr<ui::Animation> > getAnim(unsigned int bodyId, unsigned int animId);
    static unsigned int getAnimType(unsigned int bodyId);

    static boost::shared_ptr<ArtLoader> getArtLoader() { return getSingleton()->artLoader_; }
    static boost::shared_ptr<TileDataLoader> getTileDataLoader() { return getSingleton()->tileDataLoader_; }
    static boost::shared_ptr<HuesLoader> getHuesLoader() { return getSingleton()->huesLoader_; }
    static boost::shared_ptr<GumpArtLoader> getGumpArtLoader() { return getSingleton()->gumpArtLoader_; }
    static boost::shared_ptr<MapLoader> getMapLoader(unsigned int index);
    static boost::shared_ptr<StaticsLoader> getStaticsLoader(unsigned int index);
    static boost::shared_ptr<MapTexLoader> getMapTexLoader() { return getSingleton()->mapTexLoader_; }
    static boost::shared_ptr<AnimDataLoader> getAnimDataLoader() { return getSingleton()->animDataLoader_; }
    static boost::shared_ptr<AnimLoader> getAnimLoader(unsigned int index);
    static boost::shared_ptr<UniFontLoader> getUniFontLoader(unsigned int index);
    static boost::shared_ptr<ClilocLoader> getClilocLoader();
    
    static boost::shared_ptr<ui::Texture> getTexture(unsigned int source, unsigned int id);
    static boost::shared_ptr<ui::Texture> getTexture(unsigned int source, const UnicodeString& id);
    static boost::shared_ptr<ui::Texture> getTexture(const UnicodeString& source, const UnicodeString& id);
    
    static const boost::filesystem::path& getFilePathFor(const UnicodeString& string);

private:
    static Manager* singleton_;
    
    unsigned int fileFormat_;

    Manager();
    void init(Config& config);
    Manager(const Manager& copy) { }
    Manager& operator=(const Manager& copy) { return *this; }

    std::map<std::string, boost::filesystem::path> filePathMap_;
    void buildFilePathMap(Config& config);
    void addToFilePathMap(const boost::filesystem::path& directory, bool addSubdirectories = false, const UnicodeString& prefix = "");
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

    boost::shared_ptr<DefFileLoader<MobTypeDef> > mobTypesLoader_;
    boost::shared_ptr<DefFileLoader<BodyDef> > bodyDefLoader_;
    boost::shared_ptr<DefFileLoader<BodyConvDef> > bodyConvDefLoader_;
    boost::shared_ptr<DefFileLoader<PaperdollDef> > paperdollDefLoader_;
    boost::shared_ptr<DefFileLoader<GumpDef> > gumpDefLoader_;
    boost::shared_ptr<EquipConvDefLoader> equipConvDefLoader_;
    boost::shared_ptr<DefFileLoader<MountDef> > mountDefLoader_;
    boost::shared_ptr<DefFileLoader<EffectTranslationDef> > effectTranslationDefLoader_;
    boost::shared_ptr<DefFileLoader<MusicConfigDef> > musicConfigDefLoader_;

    boost::shared_ptr<ClilocLoader> clilocLoader_;
    
    boost::shared_ptr<HttpLoader> httpLoader_;
    boost::shared_ptr<FilePathLoader> filePathLoader_;
};

}
}

#endif
