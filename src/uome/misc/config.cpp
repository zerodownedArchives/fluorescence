
#include "config.hpp"

#include <iostream>
#include <fstream>

#include "logger.hpp"


namespace uome {

bool Config::getVariablesMap(const std::vector<CL_String8>& args, boost::program_options::variables_map& map) {

    // transform vector to default argc, argv
    int argc = args.size();
    char* argv[argc];
    for (int i = 0; i < argc; ++i) {
        argv[i] = const_cast<char*>(args[i].c_str());
    }

    namespace po = boost::program_options;
    po::options_description desc("Console options");
    po::options_description filesDesc("Files options");

    desc.add_options()
    ("help", "Receive this message")
    ("configfile", po::value<std::string>()->default_value(UOME_DEFAULT_CONFIG_FILE_NAME), "UO:ME config file")
    ;

    // mul file paths
    filesDesc.add_options()
    ("files.mul-directory", po::value<std::string>()->default_value("./"), "Directory containing the mul files")
    ("files.art-idx", po::value<std::string>()->default_value("artidx.mul"), "Location of artidx.mul")
    ("files.art-mul", po::value<std::string>()->default_value("art.mul"), "Location of art.mul")
    ("files.gumpart-idx", po::value<std::string>()->default_value("gumpidx.mul"), "Location of gumpidx.mul")
    ("files.gumpart-mul", po::value<std::string>()->default_value("gumpart.mul"), "Location of gumpart.mul")
    ("files.hues-mul", po::value<std::string>()->default_value("hues.mul"), "Location of hues.mul")
    ("files.tiledata-mul", po::value<std::string>()->default_value("tiledata.mul"), "Location of tiledata.mul")
    ("files.maptex-idx", po::value<std::string>()->default_value("texidx.mul"), "Location of texidx.mul")
    ("files.maptex-mul", po::value<std::string>()->default_value("texmaps.mul"), "Location of texmaps.mul")

    ("files.map0-enabled", po::value<bool>()->default_value(true), "Wheter or not map0 is enabled")
    ("files.map0-mul", po::value<std::string>()->default_value("map0.mul"), "Location of map0.mul")
    ("files.map0-dif-enabled", po::value<bool>()->default_value(true), "Wheter or not dif files are enabled for map0")
    ("files.map0-dif-offsets", po::value<std::string>()->default_value("mapdifl0.mul"), "Location of mapdifl0.mul")
    ("files.map0-dif-mul", po::value<std::string>()->default_value("mapdif0.mul"), "Location of mapdif0.mul")
    ("files.map0-size-x", po::value<unsigned int>()->default_value(768), "Horizontal block count for map0")
    ("files.map0-size-y", po::value<unsigned int>()->default_value(512), "Vertical block count for map0")

    ("files.map1-enabled", po::value<bool>()->default_value(true), "Wheter or not map1 is enabled")
    ("files.map1-mul", po::value<std::string>()->default_value("map1.mul"), "Location of map1.mul")
    ("files.map1-dif-enabled", po::value<bool>()->default_value(true), "Wheter or not dif files are enabled for map1")
    ("files.map1-dif-offsets", po::value<std::string>()->default_value("mapdifl1.mul"), "Location of mapdifl1.mul")
    ("files.map1-dif-mul", po::value<std::string>()->default_value("mapdif1.mul"), "Location of mapdif1.mul")
    ("files.map1-size-x", po::value<unsigned int>()->default_value(768), "Horizontal block count for map1")
    ("files.map1-size-y", po::value<unsigned int>()->default_value(512), "Vertical block count for map1")

    ("files.map2-enabled", po::value<bool>()->default_value(true), "Wheter or not map2 is enabled")
    ("files.map2-mul", po::value<std::string>()->default_value("map2.mul"), "Location of map2.mul")
    ("files.map2-dif-enabled", po::value<bool>()->default_value(true), "Wheter or not dif files are enabled for map2")
    ("files.map2-dif-offsets", po::value<std::string>()->default_value("mapdifl2.mul"), "Location of mapdifl2.mul")
    ("files.map2-dif-mul", po::value<std::string>()->default_value("mapdif2.mul"), "Location of mapdif2.mul")
    ("files.map2-size-x", po::value<unsigned int>()->default_value(288), "Horizontal block count for map2")
    ("files.map2-size-y", po::value<unsigned int>()->default_value(200), "Vertical block count for map2")

    ("files.map3-enabled", po::value<bool>()->default_value(true), "Wheter or not map3 is enabled")
    ("files.map3-mul", po::value<std::string>()->default_value("map3.mul"), "Location of map3.mul")
    ("files.map3-dif-enabled", po::value<bool>()->default_value(false), "Wheter or not dif files are enabled for map3")
    ("files.map3-dif-offsets", po::value<std::string>()->default_value("mapdifl3.mul"), "Location of mapdifl3.mul")
    ("files.map3-dif-mul", po::value<std::string>()->default_value("mapdif3.mul"), "Location of mapdif3.mul")
    ("files.map3-size-x", po::value<unsigned int>()->default_value(320), "Horizontal block count for map3")
    ("files.map3-size-y", po::value<unsigned int>()->default_value(256), "Vertical block count for map3")

    ("files.map4-enabled", po::value<bool>()->default_value(true), "Wheter or not map4 is enabled")
    ("files.map4-mul", po::value<std::string>()->default_value("map4.mul"), "Location of map4.mul")
    ("files.map4-dif-enabled", po::value<bool>()->default_value(false), "Wheter or not dif files are enabled for map4")
    ("files.map4-dif-offsets", po::value<std::string>()->default_value("mapdifl4.mul"), "Location of mapdifl4.mul")
    ("files.map4-dif-mul", po::value<std::string>()->default_value("mapdif4.mul"), "Location of mapdif4.mul")
    ("files.map4-size-x", po::value<unsigned int>()->default_value(181), "Horizontal block count for map4")
    ("files.map4-size-y", po::value<unsigned int>()->default_value(181), "Vertical block count for map4")

    ("files.statics0-idx", po::value<std::string>()->default_value("staidx0.mul"), "Location of staidx0.mul")
    ("files.statics0-mul", po::value<std::string>()->default_value("statics0.mul"), "Location of statics0.mul")
    ("files.statics0-dif-enabled", po::value<bool>()->default_value(true), "Wheter or not dif files are enabled for statics0")
    ("files.statics0-dif-offsets", po::value<std::string>()->default_value("stadifl0.mul"), "Location of stadifl0.mul")
    ("files.statics0-dif-idx", po::value<std::string>()->default_value("stadifi0.mul"), "Location of stadifi0.mul")
    ("files.statics0-dif-mul", po::value<std::string>()->default_value("stadif0.mul"), "Location of stadif0.mul")

    ("files.statics1-idx", po::value<std::string>()->default_value("staidx1.mul"), "Location of staidx1.mul")
    ("files.statics1-mul", po::value<std::string>()->default_value("statics1.mul"), "Location of statics1.mul")
    ("files.statics1-dif-enabled", po::value<bool>()->default_value(true), "Wheter or not dif files are enabled for statics1")
    ("files.statics1-dif-offsets", po::value<std::string>()->default_value("stadifl1.mul"), "Location of stadifl1.mul")
    ("files.statics1-dif-idx", po::value<std::string>()->default_value("stadifi1.mul"), "Location of stadifi1.mul")
    ("files.statics1-dif-mul", po::value<std::string>()->default_value("stadif1.mul"), "Location of stadif1.mul")

    ("files.statics2-idx", po::value<std::string>()->default_value("staidx2.mul"), "Location of staidx2.mul")
    ("files.statics2-mul", po::value<std::string>()->default_value("statics2.mul"), "Location of statics2.mul")
    ("files.statics2-dif-enabled", po::value<bool>()->default_value(true), "Wheter or not dif files are enabled for statics2")
    ("files.statics2-dif-offsets", po::value<std::string>()->default_value("stadifl2.mul"), "Location of stadifl2.mul")
    ("files.statics2-dif-idx", po::value<std::string>()->default_value("stadifi2.mul"), "Location of stadifi2.mul")
    ("files.statics2-dif-mul", po::value<std::string>()->default_value("stadif2.mul"), "Location of stadif2.mul")

    ("files.statics3-idx", po::value<std::string>()->default_value("staidx3.mul"), "Location of staidx3.mul")
    ("files.statics3-mul", po::value<std::string>()->default_value("statics3.mul"), "Location of statics3.mul")
    ("files.statics3-dif-enabled", po::value<bool>()->default_value(false), "Wheter or not dif files are enabled for statics3")
    ("files.statics3-dif-offsets", po::value<std::string>()->default_value("stadifl3.mul"), "Location of stadifl3.mul")
    ("files.statics3-dif-idx", po::value<std::string>()->default_value("stadifi3.mul"), "Location of stadifi3.mul")
    ("files.statics3-dif-mul", po::value<std::string>()->default_value("stadif3.mul"), "Location of stadif3.mul")

    ("files.statics4-idx", po::value<std::string>()->default_value("staidx4.mul"), "Location of staidx4.mul")
    ("files.statics4-mul", po::value<std::string>()->default_value("statics4.mul"), "Location of statics4.mul")
    ("files.statics4-dif-enabled", po::value<bool>()->default_value(false), "Wheter or not dif files are enabled for statics4")
    ("files.statics4-dif-offsets", po::value<std::string>()->default_value("stadifl4.mul"), "Location of stadifl4.mul")
    ("files.statics4-dif-idx", po::value<std::string>()->default_value("stadifi4.mul"), "Location of stadifi4.mul")
    ("files.statics4-dif-mul", po::value<std::string>()->default_value("stadif4.mul"), "Location of stadif4.mul")


    ("world.sector-check-frequency", po::value<unsigned int>()->default_value(30), "After X frames, sectors will be checked for load/delete")
    ("world.sector-add-distance", po::value<unsigned int>()->default_value(5), "Sectors closer than this are added")
    ("world.sector-remove-distance", po::value<unsigned int>()->default_value(6), "Sectors farther away than this are removed. Should be > world.sector-add-distance")
    ;

    desc.add(filesDesc);

    std::ifstream stream;

    try {
        // parse console
        po::store(po::parse_command_line(argc, argv, desc), map);

        if (map.count("help")) {
            std::cout << desc << std::endl;
            return false;
        }

        // parse file
        std::string configFilePath = map["configfile"].as<std::string>();
        stream.open(configFilePath.c_str());

        if (stream.is_open()) {
            po::store(po::parse_config_file(stream, desc), map);
            stream.close();
            return true;
        } else {
            LOGARG_CRITICAL(LOGTYPE_UNKNOWN, "Error opening config file: %s", configFilePath.c_str());
            stream.close();
            return false;
        }

        po::notify(map);
    } catch (const std::exception& ex) {
        LOGARG_CRITICAL(LOGTYPE_UNKNOWN, "Error reading config: %s", ex.what());
        stream.close();
        return false;
    }

    return false;
}
}
