
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
    ("files.art-idx", po::value<std::string>()->default_value("Artidx.mul"), "Location of artidx.mul")
    ("files.art-mul", po::value<std::string>()->default_value("Art.mul"), "Location of art.mul")
    ("files.gumpart-idx", po::value<std::string>()->default_value("Gumpidx.mul"), "Location of gumpidx.mul")
    ("files.gumpart-mul", po::value<std::string>()->default_value("Gumpart.mul"), "Location of gumpart.mul")
    ("files.hues-mul", po::value<std::string>()->default_value("Hues.mul"), "Location of hues.mul")
    ("files.tiledata-mul", po::value<std::string>()->default_value("Tiledata.mul"), "Location of tiledata.mul")
    ("files.maptex-idx", po::value<std::string>()->default_value("Texidx.mul"), "Location of texidx.mul")
    ("files.maptex-mul", po::value<std::string>()->default_value("Texmaps.mul"), "Location of texmaps.mul")
    ("files.animdata-mul", po::value<std::string>()->default_value("Animdata.mul"), "Location of animdata.mul")

    ("files.map0-enabled", po::value<bool>()->default_value(true), "Whether or not map0 is enabled")
    ("files.map0-mul", po::value<std::string>()->default_value("Map0.mul"), "Location of map0.mul")
    ("files.map0-dif-enabled", po::value<bool>()->default_value(true), "Whether or not dif files are enabled for map0")
    ("files.map0-dif-offsets", po::value<std::string>()->default_value("Mapdifl0.mul"), "Location of mapdifl0.mul")
    ("files.map0-dif-mul", po::value<std::string>()->default_value("Mapdif0.mul"), "Location of mapdif0.mul")
    ("files.map0-size-x", po::value<unsigned int>()->default_value(768), "Horizontal block count for map0")
    ("files.map0-size-y", po::value<unsigned int>()->default_value(512), "Vertical block count for map0")

    ("files.map1-enabled", po::value<bool>()->default_value(true), "Whether or not map1 is enabled")
    ("files.map1-mul", po::value<std::string>()->default_value("Map1.mul"), "Location of map1.mul")
    ("files.map1-dif-enabled", po::value<bool>()->default_value(true), "Whether or not dif files are enabled for map1")
    ("files.map1-dif-offsets", po::value<std::string>()->default_value("Mapdifl1.mul"), "Location of mapdifl1.mul")
    ("files.map1-dif-mul", po::value<std::string>()->default_value("Mapdif1.mul"), "Location of mapdif1.mul")
    ("files.map1-size-x", po::value<unsigned int>()->default_value(768), "Horizontal block count for map1")
    ("files.map1-size-y", po::value<unsigned int>()->default_value(512), "Vertical block count for map1")

    ("files.map2-enabled", po::value<bool>()->default_value(true), "Whether or not map2 is enabled")
    ("files.map2-mul", po::value<std::string>()->default_value("Map2.mul"), "Location of map2.mul")
    ("files.map2-dif-enabled", po::value<bool>()->default_value(true), "Whether or not dif files are enabled for map2")
    ("files.map2-dif-offsets", po::value<std::string>()->default_value("Mapdifl2.mul"), "Location of mapdifl2.mul")
    ("files.map2-dif-mul", po::value<std::string>()->default_value("Mapdif2.mul"), "Location of mapdif2.mul")
    ("files.map2-size-x", po::value<unsigned int>()->default_value(288), "Horizontal block count for map2")
    ("files.map2-size-y", po::value<unsigned int>()->default_value(200), "Vertical block count for map2")

    ("files.map3-enabled", po::value<bool>()->default_value(true), "Whether or not map3 is enabled")
    ("files.map3-mul", po::value<std::string>()->default_value("Map3.mul"), "Location of map3.mul")
    ("files.map3-dif-enabled", po::value<bool>()->default_value(false), "Whether or not dif files are enabled for map3")
    ("files.map3-dif-offsets", po::value<std::string>()->default_value("Mapdifl3.mul"), "Location of mapdifl3.mul")
    ("files.map3-dif-mul", po::value<std::string>()->default_value("Mapdif3.mul"), "Location of mapdif3.mul")
    ("files.map3-size-x", po::value<unsigned int>()->default_value(320), "Horizontal block count for map3")
    ("files.map3-size-y", po::value<unsigned int>()->default_value(256), "Vertical block count for map3")

    ("files.map4-enabled", po::value<bool>()->default_value(true), "Whether or not map4 is enabled")
    ("files.map4-mul", po::value<std::string>()->default_value("Map4.mul"), "Location of map4.mul")
    ("files.map4-dif-enabled", po::value<bool>()->default_value(false), "Whether or not dif files are enabled for map4")
    ("files.map4-dif-offsets", po::value<std::string>()->default_value("Mapdifl4.mul"), "Location of mapdifl4.mul")
    ("files.map4-dif-mul", po::value<std::string>()->default_value("Mapdif4.mul"), "Location of mapdif4.mul")
    ("files.map4-size-x", po::value<unsigned int>()->default_value(181), "Horizontal block count for map4")
    ("files.map4-size-y", po::value<unsigned int>()->default_value(181), "Vertical block count for map4")

    ("files.statics0-idx", po::value<std::string>()->default_value("Staidx0.mul"), "Location of staidx0.mul")
    ("files.statics0-mul", po::value<std::string>()->default_value("Statics0.mul"), "Location of statics0.mul")
    ("files.statics0-dif-enabled", po::value<bool>()->default_value(true), "Whether or not dif files are enabled for statics0")
    ("files.statics0-dif-offsets", po::value<std::string>()->default_value("Stadifl0.mul"), "Location of stadifl0.mul")
    ("files.statics0-dif-idx", po::value<std::string>()->default_value("Stadifi0.mul"), "Location of stadifi0.mul")
    ("files.statics0-dif-mul", po::value<std::string>()->default_value("Stadif0.mul"), "Location of stadif0.mul")

    ("files.statics1-idx", po::value<std::string>()->default_value("Staidx1.mul"), "Location of staidx1.mul")
    ("files.statics1-mul", po::value<std::string>()->default_value("Statics1.mul"), "Location of statics1.mul")
    ("files.statics1-dif-enabled", po::value<bool>()->default_value(true), "Whether or not dif files are enabled for statics1")
    ("files.statics1-dif-offsets", po::value<std::string>()->default_value("Stadifl1.mul"), "Location of stadifl1.mul")
    ("files.statics1-dif-idx", po::value<std::string>()->default_value("Stadifi1.mul"), "Location of stadifi1.mul")
    ("files.statics1-dif-mul", po::value<std::string>()->default_value("Stadif1.mul"), "Location of stadif1.mul")

    ("files.statics2-idx", po::value<std::string>()->default_value("Staidx2.mul"), "Location of staidx2.mul")
    ("files.statics2-mul", po::value<std::string>()->default_value("Statics2.mul"), "Location of statics2.mul")
    ("files.statics2-dif-enabled", po::value<bool>()->default_value(true), "Whether or not dif files are enabled for statics2")
    ("files.statics2-dif-offsets", po::value<std::string>()->default_value("Stadifl2.mul"), "Location of stadifl2.mul")
    ("files.statics2-dif-idx", po::value<std::string>()->default_value("Stadifi2.mul"), "Location of stadifi2.mul")
    ("files.statics2-dif-mul", po::value<std::string>()->default_value("Stadif2.mul"), "Location of stadif2.mul")

    ("files.statics3-idx", po::value<std::string>()->default_value("Staidx3.mul"), "Location of staidx3.mul")
    ("files.statics3-mul", po::value<std::string>()->default_value("Statics3.mul"), "Location of statics3.mul")
    ("files.statics3-dif-enabled", po::value<bool>()->default_value(false), "Whether or not dif files are enabled for statics3")
    ("files.statics3-dif-offsets", po::value<std::string>()->default_value("Stadifl3.mul"), "Location of stadifl3.mul")
    ("files.statics3-dif-idx", po::value<std::string>()->default_value("Stadifi3.mul"), "Location of stadifi3.mul")
    ("files.statics3-dif-mul", po::value<std::string>()->default_value("Stadif3.mul"), "Location of stadif3.mul")

    ("files.statics4-idx", po::value<std::string>()->default_value("Staidx4.mul"), "Location of staidx4.mul")
    ("files.statics4-mul", po::value<std::string>()->default_value("Statics4.mul"), "Location of statics4.mul")
    ("files.statics4-dif-enabled", po::value<bool>()->default_value(false), "Whether or not dif files are enabled for statics4")
    ("files.statics4-dif-offsets", po::value<std::string>()->default_value("Stadifl4.mul"), "Location of stadifl4.mul")
    ("files.statics4-dif-idx", po::value<std::string>()->default_value("Stadifi4.mul"), "Location of stadifi4.mul")
    ("files.statics4-dif-mul", po::value<std::string>()->default_value("Stadif4.mul"), "Location of stadif4.mul")

    ("files.body-def", po::value<std::string>()->default_value("Body.def"), "Location of body.def")
    ("files.bodyconf-def", po::value<std::string>()->default_value("Bodyconv"), "Location of bodyconv.def")

    ("files.anim0-enabled", po::value<bool>()->default_value(true), "Wheter or not anim is enabled")
    ("files.anim0-idx", po::value<std::string>()->default_value("Anim.idx"), "Location of anim.idx")
    ("files.anim0-mul", po::value<std::string>()->default_value("Anim.mul"), "Location of anim.mul")
    ("files.anim0-highdetail", po::value<unsigned int>()->default_value(200), "High detail count")
    ("files.anim0-lowdetail", po::value<unsigned int>()->default_value(200), "Low detail count")

    ("files.anim1-enabled", po::value<bool>()->default_value(true), "Wheter or not anim1 is enabled")
    ("files.anim1-idx", po::value<std::string>()->default_value("Anim.idx"), "Location of anim1.idx")
    ("files.anim1-mul", po::value<std::string>()->default_value("Anim.mul"), "Location of anim1.mul")
    ("files.anim1-highdetail", po::value<unsigned int>()->default_value(200), "High detail count")
    ("files.anim1-lowdetail", po::value<unsigned int>()->default_value(200), "Low detail count")

    ("files.anim2-enabled", po::value<bool>()->default_value(true), "Wheter or not anim2 is enabled")
    ("files.anim2-idx", po::value<std::string>()->default_value("Anim2.idx"), "Location of anim2.idx")
    ("files.anim2-mul", po::value<std::string>()->default_value("Anim2.mul"), "Location of anim2.mul")
    ("files.anim2-highdetail", po::value<unsigned int>()->default_value(200), "High detail count")
    ("files.anim2-lowdetail", po::value<unsigned int>()->default_value(200), "Low detail count")

    ("files.anim3-enabled", po::value<bool>()->default_value(true), "Wheter or not anim3 is enabled")
    ("files.anim3-idx", po::value<std::string>()->default_value("Anim3.idx"), "Location of anim3.idx")
    ("files.anim3-mul", po::value<std::string>()->default_value("Anim3.mul"), "Location of anim3.mul")
    ("files.anim3-highdetail", po::value<unsigned int>()->default_value(200), "High detail count")
    ("files.anim3-lowdetail", po::value<unsigned int>()->default_value(200), "Low detail count")

    ("files.anim4-enabled", po::value<bool>()->default_value(true), "Wheter or not anim4 is enabled")
    ("files.anim4-idx", po::value<std::string>()->default_value("Anim4.idx"), "Location of anim4.idx")
    ("files.anim4-mul", po::value<std::string>()->default_value("Anim4.mul"), "Location of anim4.mul")
    ("files.anim4-highdetail", po::value<unsigned int>()->default_value(200), "High detail count")
    ("files.anim4-lowdetail", po::value<unsigned int>()->default_value(200), "Low detail count")

    ("files.anim5-enabled", po::value<bool>()->default_value(true), "Wheter or not anim5 is enabled")
    ("files.anim5-idx", po::value<std::string>()->default_value("Anim5.idx"), "Location of anim5.idx")
    ("files.anim5-mul", po::value<std::string>()->default_value("Anim5.mul"), "Location of anim5.mul")
    ("files.anim5-highdetail", po::value<unsigned int>()->default_value(200), "High detail count")
    ("files.anim5-lowdetail", po::value<unsigned int>()->default_value(200), "Low detail count")


    ("ui.themes-directory", po::value<std::string>()->default_value("themes"), "Directory containing the themes for UO:ME")
    ("ui.default-theme", po::value<std::string>()->default_value("GUIThemeBasic"), "Default ui theme")
    ("ui.gumps-directory", po::value<std::string>()->default_value("gumps"), "Directory containing the xml gump files")
    ("ui.cursor-artid-start", po::value<unsigned int>()->default_value(0x206A), "Begin art id of cursor sequence")
    ("ui.cursor-artid-start-warmode", po::value<unsigned int>()->default_value(0x2053), "Begin art id of cursor sequence (warmode)")
    ("ui.doubleclick-timeout-ms", po::value<unsigned int>()->default_value(300), "Maximum interval between two clicks to be counted as double click")
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
