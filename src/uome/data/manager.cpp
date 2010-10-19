
#include "manager.hpp"

namespace uome {
namespace data {

Manager* Manager::singleton_= NULL;

bool Manager::create(const boost::program_options::variables_map& config) {
    if (!singleton_) {
        try {
            singleton_ = new Manager(config);
        } catch (const std::exception& ex) {
            LOGARG_CRITICAL(LOGTYPE_DATA, "Error initializing data::Manager: %s", ex.what());
            return false;
        }
    }

    return true;
}

Manager* Manager::getSingleton() {
    if (!singleton_) {
        throw Exception("uome::data::Manager Singleton not created yet");
    }

    return singleton_;
}

Manager::Manager(const boost::program_options::variables_map& config) throw(Exception) {
    boost::filesystem::path mulDirPath = config["files.mul-directory"].as<std::string>();
    if (!boost::filesystem::exists(mulDirPath) || !boost::filesystem::is_directory(mulDirPath)) {
        throw Exception("Invalid mul directory");
    }

    boost::filesystem::path idxPath;
    boost::filesystem::path path;

    path = getPathFor(config, "files.tiledata-mul");
    LOGARG_INFO(LOGTYPE_DATA, "Opening tiledata from %s", path.string().c_str());
    tileDataLoader_.reset(new TileDataLoader(path));

    idxPath = getPathFor(config, "files.art-idx");
    path = getPathFor(config, "files.art-mul");
    LOGARG_INFO(LOGTYPE_DATA, "Opening art from idx=%s mul=%s", idxPath.string().c_str(), path.string().c_str());
    artLoader_.reset(new ArtLoader(idxPath, path));

    path = getPathFor(config, "files.hues-mul");
    LOGARG_INFO(LOGTYPE_DATA, "Opening hues from %s", path.string().c_str());
    huesLoader_.reset(new HuesLoader(path));

    idxPath = getPathFor(config, "files.gumpart-idx");
    path = getPathFor(config, "files.gumpart-mul");
    LOGARG_INFO(LOGTYPE_DATA, "Opening gump art from idx=%s mul=%s", idxPath.string().c_str(), path.string().c_str());
    gumpArtLoader_.reset(new GumpArtLoader(idxPath, path));
}

Manager::~Manager() {
}

boost::filesystem::path Manager::getPathFor(const boost::program_options::variables_map& config, const char* configValue) {
    static boost::filesystem::path mulDirPath = config["files.mul-directory"].as<std::string>();
    boost::filesystem::path path = config[configValue].as<std::string>();
    if (!boost::filesystem::exists(path)) {
        path = mulDirPath / path;
    }

    return path;
}



}
}
