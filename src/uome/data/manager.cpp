
#include "manager.hpp"

#include "tiledataloader.hpp"
#include "artloader.hpp"
#include "huesloader.hpp"
#include "gumpartloader.hpp"
#include "maploader.hpp"
#include "staticsloader.hpp"
#include "maptexloader.hpp"

#include <ui/singletextureprovider.hpp>

namespace uome {
namespace data {

Manager* Manager::singleton_= NULL;

bool Manager::create(const boost::program_options::variables_map& config) {
    if (!singleton_) {
        try {
            singleton_ = new Manager();
            singleton_->init(config);
        } catch (const std::exception& ex) {
            LOGARG_CRITICAL(LOGTYPE_DATA, "Error initializing data::Manager: %s", ex.what());
            return false;
        }
    }

    return true;
}

void Manager::destroy() {
    if (singleton_) {
        delete singleton_;
        singleton_ = NULL;
    }
}

Manager* Manager::getSingleton() {
    if (!singleton_) {
        throw Exception("uome::data::Manager Singleton not created yet");
    }

    return singleton_;
}

Manager::Manager() {
}

void Manager::init(const boost::program_options::variables_map& config) {
    boost::filesystem::path mulDirPath = config["files.mul-directory"].as<std::string>();
    if (!boost::filesystem::exists(mulDirPath) || !boost::filesystem::is_directory(mulDirPath)) {
        throw Exception("Invalid mul directory");
    }

    boost::filesystem::path idxPath;
    boost::filesystem::path path;
    boost::filesystem::path difOffsetsPath;
    boost::filesystem::path difIdxPath;
    boost::filesystem::path difPath;

    path = getPathFor(config, "files.tiledata-mul");
    LOGARG_INFO(LOGTYPE_DATA, "Opening tiledata from %s", path.string().c_str());
    tileDataLoader_.reset(new TileDataLoader(path));

    path = getPathFor(config, "files.hues-mul");
    LOGARG_INFO(LOGTYPE_DATA, "Opening hues from %s", path.string().c_str());
    huesLoader_.reset(new HuesLoader(path));

    idxPath = getPathFor(config, "files.maptex-idx");
    path = getPathFor(config, "files.maptex-mul");
    LOGARG_INFO(LOGTYPE_DATA, "Opening maptex from idx=%s mul=%s", idxPath.string().c_str(), path.string().c_str());
    mapTexLoader_.reset(new MapTexLoader(idxPath, path));

    idxPath = getPathFor(config, "files.art-idx");
    path = getPathFor(config, "files.art-mul");
    LOGARG_INFO(LOGTYPE_DATA, "Opening art from idx=%s mul=%s", idxPath.string().c_str(), path.string().c_str());
    artLoader_.reset(new ArtLoader(idxPath, path));

    idxPath = getPathFor(config, "files.gumpart-idx");
    path = getPathFor(config, "files.gumpart-mul");
    LOGARG_INFO(LOGTYPE_DATA, "Opening gump art from idx=%s mul=%s", idxPath.string().c_str(), path.string().c_str());
    gumpArtLoader_.reset(new GumpArtLoader(idxPath, path));


    char* mapConfigEnabled = strdup("files.map0-enabled");
    char* mapConfigMulPath = strdup("files.map0-mul");
    char* mapConfigSizeX = strdup("files.map0-size-x");
    char* mapConfigSizeY = strdup("files.map0-size-y");
    char* mapConfigDifEnabled = strdup("files.map0-dif-enabled");
    char* mapConfigDifOffsetsPath = strdup("files.map0-dif-offsets");
    char* mapConfigDifMulPath = strdup("files.map0-dif-mul");

    char* staticsConfigIdxPath = strdup("files.statics0-idx");
    char* staticsConfigMulPath = strdup("files.statics0-mul");
    char* staticsConfigDifEnabled = strdup("files.statics0-dif-enabled");
    char* staticsConfigDifOffsetsPath = strdup("files.statics0-dif-offsets");
    char* staticsConfigDifIdxPath = strdup("files.statics0-dif-idx");
    char* staticsConfigDifMulPath = strdup("files.statics0-dif-mul");

    unsigned int blockCountX;
    unsigned int blockCountY;

    char indexChar = '0';
    for (unsigned int index = 0; index < 5; ++index, ++indexChar) {
        mapConfigEnabled[9] = indexChar;

        if (!config[mapConfigEnabled].as<bool>()) {
            continue;
        }

        mapConfigMulPath[9] = indexChar;
        mapConfigSizeX[9] = indexChar;
        mapConfigSizeY[9] = indexChar;
        mapConfigDifEnabled[9] = indexChar;
        mapConfigDifOffsetsPath[9] = indexChar;
        mapConfigDifMulPath[9] = indexChar;

        staticsConfigIdxPath[13] = indexChar;
        staticsConfigMulPath[13] = indexChar;
        staticsConfigDifEnabled[13] = indexChar;
        staticsConfigDifOffsetsPath[13] = indexChar;
        staticsConfigDifIdxPath[13] = indexChar;
        staticsConfigDifMulPath[13] = indexChar;

        path = getPathFor(config, mapConfigMulPath);
        blockCountX = config[mapConfigSizeX].as<unsigned int>();
        blockCountY = config[mapConfigSizeY].as<unsigned int>();
        if (config[mapConfigDifEnabled].as<bool>()) {
            difOffsetsPath = getPathFor(config, mapConfigDifOffsetsPath);
            difPath = getPathFor(config, mapConfigDifMulPath);
            LOGARG_INFO(LOGTYPE_DATA, "Opening map%c from mul=%s, dif-offsets=%s, dif=%s, blockCountX=%u, blockCountY=%u", indexChar, path.string().c_str(), difOffsetsPath.string().c_str(), difPath.string().c_str(), blockCountX, blockCountY);
            mapLoader_[index].reset(new MapLoader(path, difOffsetsPath, difPath, blockCountX, blockCountY));
        } else {
            LOGARG_INFO(LOGTYPE_DATA, "Opening map%c from mul=%s, difs disabled, blockCountX=%u, blockCountY=%u", indexChar, path.string().c_str(), blockCountX, blockCountY);
            mapLoader_[index].reset(new MapLoader(path, blockCountX, blockCountY));
        }

        if (!fallbackMapLoader_.get()) {
            fallbackMapLoader_ = mapLoader_[index];
        }

        idxPath = getPathFor(config, staticsConfigIdxPath);
        path = getPathFor(config, staticsConfigMulPath);
        if (config[staticsConfigDifEnabled].as<bool>()) {
            difOffsetsPath = getPathFor(config, staticsConfigDifOffsetsPath);
            difIdxPath = getPathFor(config, staticsConfigDifIdxPath);
            difPath = getPathFor(config, staticsConfigDifMulPath);
            LOGARG_INFO(LOGTYPE_DATA, "Opening statics%c from idx=%s, mul=%s, dif-offsets=%s, dif-idx=%s, dif=%s",
                    indexChar, idxPath.string().c_str(), path.string().c_str(),
                    difOffsetsPath.string().c_str(), difIdxPath.string().c_str(), difPath.string().c_str());
            staticsLoader_[index].reset(new StaticsLoader(idxPath, path, difOffsetsPath, difIdxPath, difPath, blockCountX, blockCountY));
        } else {
            LOGARG_INFO(LOGTYPE_DATA, "Opening statics%c from idx=%s, mul=%s, difs disabled,", indexChar, idxPath.string().c_str(), path.string().c_str());
            staticsLoader_[index].reset(new StaticsLoader(idxPath, path, blockCountX, blockCountY));
        }

        if (!fallbackStaticsLoader_.get()) {
            fallbackStaticsLoader_ = staticsLoader_[index];
        }
    }

    free(mapConfigEnabled);
    free(mapConfigMulPath);
    free(mapConfigSizeX);
    free(mapConfigSizeY);
    free(mapConfigDifEnabled);
    free(mapConfigDifOffsetsPath);
    free(mapConfigDifMulPath);
    free(staticsConfigIdxPath);
    free(staticsConfigMulPath);
    free(staticsConfigDifEnabled);
    free(staticsConfigDifOffsetsPath);
    free(staticsConfigDifIdxPath);
    free(staticsConfigDifMulPath);


}

Manager::~Manager() {
}

boost::filesystem::path Manager::getPathFor(const boost::program_options::variables_map& config, const char* configValue) {
    static boost::filesystem::path mulDirPath = config["files.mul-directory"].as<std::string>();
    if (config.count(configValue) <= 0) {
        LOGARG_ERROR(LOGTYPE_DATA, "Trying to read unknown config vaule %s", configValue);
        throw Exception("Unknown config key: ", configValue);
    }
    boost::filesystem::path path = config[configValue].as<std::string>();
    if (!boost::filesystem::exists(path)) {
        path = mulDirPath / path;
    }

    return path;
}

MapLoader* Manager::getMapLoader(unsigned int index) {
    if (index > 4) {
        LOGARG_WARN(LOGTYPE_DATA, "Trying to access map loader for map index %u", index);
        index = 0;
    }

    Manager* sing = getSingleton();

    MapLoader* ret = sing->mapLoader_[index].get();
    if (sing->mapLoader_[index].get()) {
        return ret;
    } else {
        LOGARG_WARN(LOGTYPE_DATA, "Trying to access uninitialized map index %u", index);
        return sing->fallbackMapLoader_.get();
    }
}

StaticsLoader* Manager::getStaticsLoader(unsigned int index) {
    if (index > 4) {
        LOGARG_WARN(LOGTYPE_DATA, "Trying to access statics loader for map index %u", index);
        index = 0;
    }

    Manager* sing = getSingleton();

    StaticsLoader* ret = sing->staticsLoader_[index].get();
    if (sing->staticsLoader_[index].get()) {
        return ret;
    } else {
        LOGARG_WARN(LOGTYPE_DATA, "Trying to access uninitialized statics index %u", index);
        return sing->fallbackStaticsLoader_.get();
    }
}

boost::shared_ptr<ui::TextureProvider> Manager::getItemTextureProvider(unsigned int artId) {
    // check for entry in animdata
    // if exists, load complex textureprovider
    // if not, just load the simple one

    boost::shared_ptr<ui::SingleTextureProvider> ret(new ui::SingleTextureProvider(artId));
    return ret;
}


}
}
