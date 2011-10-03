
#include "manager.hpp"

#include "tiledataloader.hpp"
#include "artloader.hpp"
#include "huesloader.hpp"
#include "gumpartloader.hpp"
#include "maploader.hpp"
#include "staticsloader.hpp"
#include "maptexloader.hpp"
#include "animdataloader.hpp"
#include "animloader.hpp"
#include "mobtypesloader.hpp"

#include <ui/singletextureprovider.hpp>
#include <ui/animdatatextureprovider.hpp>

#include <misc/filenamecaseconverter.hpp>

#include <typedefs.hpp>

namespace fluo {
namespace data {

Manager* Manager::singleton_= NULL;

bool Manager::create(Config& config) {
    if (!singleton_) {
        try {
            singleton_ = new Manager();
            singleton_->init(config);
        } catch (const std::exception& ex) {
            LOG_EMERGENCY << "Error initializing data::Manager: " << ex.what() << std::endl;
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
        throw Exception("fluo::data::Manager Singleton not created yet");
    }

    return singleton_;
}

Manager::Manager() {
}

void Manager::init(Config& config) {
    boost::filesystem::path mulDirPath = config["/fluo/files/mul-directory@path"].asPath();
    if (!boost::filesystem::exists(mulDirPath) || !boost::filesystem::is_directory(mulDirPath)) {
        throw Exception("Invalid mul directory");
    }

    LOG_INFO << "Converting filenames to lower case" << std::endl;
    if (!misc::FileNameCaseConverter::convert(mulDirPath)) {
        LOG_EMERGENCY << "Invalid mul directory" << std::endl;
        throw std::exception();
    }

    boost::filesystem::path idxPath;
    boost::filesystem::path path;
    boost::filesystem::path difOffsetsPath;
    boost::filesystem::path difIdxPath;
    boost::filesystem::path difPath;

    path = getPathFor(config, "/fluo/files/tiledata@filename");
    LOG_INFO << "Opening tiledata from " << path << std::endl;
    tileDataLoader_.reset(new TileDataLoader(path));

    path = getPathFor(config, "/fluo/files/hues@filename");
    LOG_INFO << "Opening hues from " << path << std::endl;
    huesLoader_.reset(new HuesLoader(path));

    idxPath = getPathFor(config, "/fluo/files/texmaps@filename-idx");
    path = getPathFor(config, "/fluo/files/texmaps@filename-mul");
    LOG_INFO << "Opening maptex from idx=" << idxPath << " mul=" << path << std::endl;
    mapTexLoader_.reset(new MapTexLoader(idxPath, path));

    idxPath = getPathFor(config, "/fluo/files/art@filename-idx");
    path = getPathFor(config, "/fluo/files/art@filename-mul");
    LOG_INFO << "Opening art from idx=" << idxPath << " mul=" << path << std::endl;
    artLoader_.reset(new ArtLoader(idxPath, path));

    idxPath = getPathFor(config, "/fluo/files/gumpart@filename-idx");
    path = getPathFor(config, "/fluo/files/gumpart@filename-mul");
    LOG_INFO << "Opening gump art from idx=" << idxPath << " mul=" << path << std::endl;
    gumpArtLoader_.reset(new GumpArtLoader(idxPath, path));

    path = getPathFor(config, "/fluo/files/animdata@filename");
    LOG_INFO << "Opening animdata from mul=" << path << std::endl;
    animDataLoader_.reset(new AnimDataLoader(path));


    char* mapConfigEnabled = strdup("/fluo/files/map0@enabled");
    char* mapConfigMulPath = strdup("/fluo/files/map0@filename");
    char* mapConfigSizeX = strdup("/fluo/files/map0@width");
    char* mapConfigSizeY = strdup("/fluo/files/map0@height");
    char* mapConfigDifEnabled = strdup("/fluo/files/map0/difs@enabled");
    char* mapConfigDifOffsetsPath = strdup("/fluo/files/map0/difs@filename-offsets");
    char* mapConfigDifMulPath = strdup("/fluo/files/map0/difs@filename-mul");

    char* staticsConfigIdxPath = strdup("/fluo/files/map0/statics@filename-idx");
    char* staticsConfigMulPath = strdup("/fluo/files/map0/statics@filename-mul");
    char* staticsConfigDifEnabled = strdup("/fluo/files/map0/statics/difs@enabled");
    char* staticsConfigDifOffsetsPath = strdup("/fluo/files/map0/statics/difs@filename-offsets");
    char* staticsConfigDifIdxPath = strdup("/fluo/files/map0/statics/difs@filename-idx");
    char* staticsConfigDifMulPath = strdup("/fluo/files/map0/statics/difs@filename-mul");

    unsigned int blockCountX;
    unsigned int blockCountY;

    char indexChar = '0';
    unsigned int digitIndex = 15;
    for (unsigned int index = 0; index < 5; ++index, ++indexChar) {
        mapConfigEnabled[digitIndex] = indexChar;

        if (!config[mapConfigEnabled].asBool()) {
            continue;
        }

        mapConfigMulPath[digitIndex] = indexChar;
        mapConfigSizeX[digitIndex] = indexChar;
        mapConfigSizeY[digitIndex] = indexChar;
        mapConfigDifEnabled[digitIndex] = indexChar;
        mapConfigDifOffsetsPath[digitIndex] = indexChar;
        mapConfigDifMulPath[digitIndex] = indexChar;

        staticsConfigIdxPath[digitIndex] = indexChar;
        staticsConfigMulPath[digitIndex] = indexChar;
        staticsConfigDifEnabled[digitIndex] = indexChar;
        staticsConfigDifOffsetsPath[digitIndex] = indexChar;
        staticsConfigDifIdxPath[digitIndex] = indexChar;
        staticsConfigDifMulPath[digitIndex] = indexChar;

        path = getPathFor(config, mapConfigMulPath);
        blockCountX = config[mapConfigSizeX].asInt();
        blockCountY = config[mapConfigSizeY].asInt();
        if (boost::filesystem::exists(path)) {
            if (config[mapConfigDifEnabled].asBool()) {
                difOffsetsPath = getPathFor(config, mapConfigDifOffsetsPath);
                difPath = getPathFor(config, mapConfigDifMulPath);
                LOG_INFO << "Opening map" << indexChar << " from mul=" << path << ", dif-offsets=" << difOffsetsPath <<
                        ", dif=" << difPath << ", blockCountX=" << blockCountX << ", blockCountY=" << blockCountY << std::endl;
                mapLoader_[index].reset(new MapLoader(path, difOffsetsPath, difPath, blockCountX, blockCountY));
            } else {
                LOG_INFO << "Opening map" << indexChar << " from mul=" << path << ", difs disabled, blockCountX=" <<
                        blockCountX << ", blockCountY=" << blockCountY << std::endl;
                mapLoader_[index].reset(new MapLoader(path, blockCountX, blockCountY));
            }

            if (!fallbackMapLoader_.get()) {
                fallbackMapLoader_ = mapLoader_[index];
            }
        } else {
            LOG_WARN << "Unable to open map" << indexChar << " from mul=" << path << std::endl;
        }

        idxPath = getPathFor(config, staticsConfigIdxPath);
        path = getPathFor(config, staticsConfigMulPath);

        if (boost::filesystem::exists(idxPath) && boost::filesystem::exists(path)) {
            if (config[staticsConfigDifEnabled].asBool()) {
                difOffsetsPath = getPathFor(config, staticsConfigDifOffsetsPath);
                difIdxPath = getPathFor(config, staticsConfigDifIdxPath);
                difPath = getPathFor(config, staticsConfigDifMulPath);
                LOG_INFO << "Opening statics" << indexChar << " from idx=" << idxPath << ", mul=" << path << ", dif-offsets=" << difOffsetsPath <<
                        ", dif-idx=" << difIdxPath << ", dif=" << difPath << std::endl;
                staticsLoader_[index].reset(new StaticsLoader(idxPath, path, difOffsetsPath, difIdxPath, difPath, blockCountX, blockCountY));
            } else {
                LOG_INFO << "Opening statics" << indexChar << " from idx=" << idxPath << ", mul=" << path << ", difs disabled" << std::endl;
                staticsLoader_[index].reset(new StaticsLoader(idxPath, path, blockCountX, blockCountY));
            }

            if (!fallbackStaticsLoader_.get()) {
                fallbackStaticsLoader_ = staticsLoader_[index];
            }
        } else {
            LOG_WARN << "Unable to open statics" << indexChar << " from idx=" << idxPath << ", mul=" << path << std::endl;
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



    char* animConfigEnabled = strdup("/fluo/files/anim0@enabled");
    char* animConfigIdxPath = strdup("/fluo/files/anim0@filename-idx");
    char* animConfigMulPath = strdup("/fluo/files/anim0@filename-mul");
    char* animConfigHighDetailCount = strdup("/fluo/files/anim0@highdetail");
    char* animConfigLowDetailCount = strdup("/fluo/files/anim0@lowdetail");

    unsigned int highDetailCount;
    unsigned int lowDetailCount;

    indexChar = '0';
    digitIndex = 16;
    for (unsigned int index = 0; index < 5; ++index, ++indexChar) {
        animConfigEnabled[digitIndex] = indexChar;

        if (!config[animConfigEnabled].asBool()) {
            continue;
        }

        animConfigIdxPath[digitIndex] = indexChar;
        animConfigMulPath[digitIndex] = indexChar;
        animConfigHighDetailCount[digitIndex] = indexChar;
        animConfigLowDetailCount[digitIndex] = indexChar;

        idxPath = getPathFor(config, animConfigIdxPath);
        path = getPathFor(config, animConfigMulPath);
        highDetailCount = config[animConfigHighDetailCount].asInt();
        lowDetailCount = config[animConfigLowDetailCount].asInt();

        if (boost::filesystem::exists(idxPath) && boost::filesystem::exists(path)) {
            LOG_WARN << "Unable to open anim" << indexChar << " from idx=" << idxPath << ", mul=" << path << ", high-detail=" <<
                    highDetailCount << ", low-detail=" << lowDetailCount << std::endl;
            animLoader_[index].reset(new AnimLoader(idxPath, path, highDetailCount, lowDetailCount));

            if (!fallbackAnimLoader_.get()) {
                fallbackAnimLoader_ = animLoader_[index];
            }
        } else {
            LOG_WARN << "Unable to open anim" << indexChar << " from idx=" << idxPath << ", mul=" << path << std::endl;
        }
    }

    free(animConfigEnabled);
    free(animConfigIdxPath);
    free(animConfigMulPath);
    free(animConfigHighDetailCount);
    free(animConfigLowDetailCount);


    path = getPathFor(config, "/fluo/files/mobtypes@filename");
    LOG_INFO << "Opening mobtypes.txt from path=" << path << std::endl;
    mobTypesLoader_.reset(new MobTypesLoader(path));
}

Manager::~Manager() {
    LOG_INFO << "data::Manager shutdown" << std::endl;
}

boost::filesystem::path Manager::getPathFor(Config& config, const char* configValue) {
    static boost::filesystem::path mulDirPath = config["/fluo/files/mul-directory@path"].asPath();
    if (!config.exists(configValue)) {
        LOG_ERROR << "Trying to read unknown config vaule " << configValue << std::endl;
        throw Exception("Unknown config key: ", configValue);
    }

    boost::filesystem::path path = config[configValue].asPath();
    if (!boost::filesystem::exists(path)) {
        path = mulDirPath / path;
    }

    return path;
}

MapLoader* Manager::getMapLoader(unsigned int index) {
    if (index > 4) {
        LOG_WARN << "Trying to access map loader for map index " << index << std::endl;
        index = 0;
    }

    Manager* sing = getSingleton();

    MapLoader* ret = sing->mapLoader_[index].get();
    if (ret) {
        return ret;
    } else {
        LOG_WARN << "Trying to access uninitialized map index " << index << std::endl;
        return sing->fallbackMapLoader_.get();
    }
}

StaticsLoader* Manager::getStaticsLoader(unsigned int index) {
    if (index > 4) {
        LOG_WARN << "Trying to access statics loader for map index " << index << std::endl;
        index = 0;
    }

    Manager* sing = getSingleton();

    StaticsLoader* ret = sing->staticsLoader_[index].get();
    if (ret) {
        return ret;
    } else {
        LOG_WARN << "Trying to access uninitialized statics index " << index << std::endl;
        return sing->fallbackStaticsLoader_.get();
    }
}

boost::shared_ptr<ui::TextureProvider> Manager::getItemTextureProvider(unsigned int artId) {
    boost::shared_ptr<ui::TextureProvider> ret;

    // check for animate flag in tiledata
    if (getTileDataLoader()->getStaticTileInfo(artId)->animation()) {
        // if exists, load complex textureprovider
        ret.reset(new ui::AnimDataTextureProvider(artId));
    } else {
        // if not, just load the simple one
        ret.reset(new ui::SingleTextureProvider(artId));
    }

    return ret;
}

std::vector<boost::shared_ptr<ui::Animation> > Manager::getAnim(unsigned int bodyId, unsigned int animId) {
    // TODO check .def files for correct anim file
    std::vector<boost::shared_ptr<ui::Animation> > ret;

    boost::shared_ptr<ui::Animation> tmpDown = getSingleton()->animLoader_[0]->getAnimation(bodyId, animId, 0);
    boost::shared_ptr<ui::Animation> tmpDownLeft = getSingleton()->animLoader_[0]->getAnimation(bodyId, animId, 1);
    boost::shared_ptr<ui::Animation> tmpLeft = getSingleton()->animLoader_[0]->getAnimation(bodyId, animId, 2);
    boost::shared_ptr<ui::Animation> tmpUpLeft = getSingleton()->animLoader_[0]->getAnimation(bodyId, animId, 3);
    boost::shared_ptr<ui::Animation> tmpUp = getSingleton()->animLoader_[0]->getAnimation(bodyId, animId, 4);

    // mirrored
    ret.push_back(tmpUpLeft);
    ret.push_back(tmpLeft);
    ret.push_back(tmpDownLeft);

    ret.push_back(tmpDown);
    ret.push_back(tmpDownLeft);
    ret.push_back(tmpLeft);
    ret.push_back(tmpUpLeft);
    ret.push_back(tmpUp);

    return ret;
}

AnimLoader* Manager::getAnimLoader(unsigned int index) {
    if (index > 5) {
        LOG_WARN << "Trying to access anim loader with index " << index << std::endl;
        index = 0;
    }

    Manager* sing = getSingleton();

    AnimLoader* ret = sing->animLoader_[index].get();
    if (ret) {
        return ret;
    } else {
        LOG_WARN << "Trying to access uninitialized anim loader with index " << index << std::endl;
        return sing->fallbackAnimLoader_.get();
    }
}


}
}
