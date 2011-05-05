
#include "config.hpp"

#include <iostream>
#include <fstream>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp>

#include "logger.hpp"


namespace uome {

Config::Config() {
    // file paths
    // TODO: reset path
    variablesMap_["uome/files/mul-directory@path"].setPath("/home/games/uo2d/", true);
    variablesMap_["uome/files/art@filename-idx"].setPath("Artidx.mul", true);
    variablesMap_["uome/files/art@filename-mul"].setPath("Art.mul", true);
    variablesMap_["uome/files/gumpart@filename-idx"].setPath("Gumpidx.mul", true);
    variablesMap_["uome/files/gumpart@filename-mul"].setPath("Gumpart.mul", true);
    variablesMap_["uome/files/hues@filename"].setPath("Hues.mul", true);
    variablesMap_["uome/files/tiledata@filename"].setPath("Tiledata.mul", true);
    variablesMap_["uome/files/texmaps@filename-idx"].setPath("Texidx.mul", true);
    variablesMap_["uome/files/texmaps@filename-mul"].setPath("Texmaps.mul", true);
    variablesMap_["uome/files/animdata@filename"].setPath("Animdata.mul", true);


    // maps
    variablesMap_["uome/files/map0@enabled"].setBool(true, true);
    variablesMap_["uome/files/map0@filename"].setPath("Map0.mul", true);
    variablesMap_["uome/files/map0/difs@enabled"].setBool(true, true);
    variablesMap_["uome/files/map0/difs@filename-offsets"].setPath("Mapdifl0.mul", true);
    variablesMap_["uome/files/map0/difs@filename-mul"].setPath("Mapdif0.mul", true);
    variablesMap_["uome/files/map0@width"].setUint(768, true);
    variablesMap_["uome/files/map0@height"].setUint(512, true);

    variablesMap_["uome/files/map0/statics@filename-idx"].setPath("Staidx0.mul", true);
    variablesMap_["uome/files/map0/statics@filename-mul"].setPath("Statics0.mul", true);
    variablesMap_["uome/files/map0/statics/difs@enabled"].setBool(true, true);
    variablesMap_["uome/files/map0/statics/difs@filename-offsets"].setPath("Stadifl0.mul", true);
    variablesMap_["uome/files/map0/statics/difs@filename-idx"].setPath("Stadifi0.mul", true);
    variablesMap_["uome/files/map0/statics/difs@filename-mul"].setPath("Stadif0.mul", true);


    variablesMap_["uome/files/map1@enabled"].setBool(true, true);
    variablesMap_["uome/files/map1@filename"].setPath("Map0.mul", true);
    variablesMap_["uome/files/map1/difs@enabled"].setBool(true, true);
    variablesMap_["uome/files/map1/difs@filename-offsets"].setPath("Mapdifl1.mul", true);
    variablesMap_["uome/files/map1/difs@filename-mul"].setPath("Mapdif1.mul", true);
    variablesMap_["uome/files/map1@width"].setUint(768, true);
    variablesMap_["uome/files/map1@height"].setUint(512, true);

    variablesMap_["uome/files/map1/statics@filename-idx"].setPath("Staidx0.mul", true);
    variablesMap_["uome/files/map1/statics@filename-mul"].setPath("Statics0.mul", true);
    variablesMap_["uome/files/map1/statics/difs@enabled"].setBool(true, true);
    variablesMap_["uome/files/map1/statics/difs@filename-offsets"].setPath("Stadifl1.mul", true);
    variablesMap_["uome/files/map1/statics/difs@filename-idx"].setPath("Stadifi1.mul", true);
    variablesMap_["uome/files/map1/statics/difs@filename-mul"].setPath("Stadif1.mul", true);


    variablesMap_["uome/files/map2@enabled"].setBool(true, true);
    variablesMap_["uome/files/map2@filename"].setPath("Map2.mul", true);
    variablesMap_["uome/files/map2/difs@enabled"].setBool(true, true);
    variablesMap_["uome/files/map2/difs@filename-offsets"].setPath("Mapdifl2.mul", true);
    variablesMap_["uome/files/map2/difs@filename-mul"].setPath("Mapdif2.mul", true);
    variablesMap_["uome/files/map2@width"].setUint(288, true);
    variablesMap_["uome/files/map2@height"].setUint(200, true);

    variablesMap_["uome/files/map2/statics@filename-idx"].setPath("Staidx2.mul", true);
    variablesMap_["uome/files/map2/statics@filename-mul"].setPath("Statics2.mul", true);
    variablesMap_["uome/files/map2/statics/difs@enabled"].setBool(true, true);
    variablesMap_["uome/files/map2/statics/difs@filename-offsets"].setPath("Stadifl2.mul", true);
    variablesMap_["uome/files/map2/statics/difs@filename-idx"].setPath("Stadifi2.mul", true);
    variablesMap_["uome/files/map2/statics/difs@filename-mul"].setPath("Stadif2.mul", true);


    variablesMap_["uome/files/map3@enabled"].setBool(true, true);
    variablesMap_["uome/files/map3@filename"].setPath("Map3.mul", true);
    variablesMap_["uome/files/map3/difs@enabled"].setBool(true, true);
    variablesMap_["uome/files/map3/difs@filename-offsets"].setPath("Mapdifl3.mul", true);
    variablesMap_["uome/files/map3/difs@filename-mul"].setPath("Mapdif3.mul", true);
    variablesMap_["uome/files/map3@width"].setUint(320, true);
    variablesMap_["uome/files/map3@height"].setUint(256, true);

    variablesMap_["uome/files/map3/statics@filename-idx"].setPath("Staidx3.mul", true);
    variablesMap_["uome/files/map3/statics@filename-mul"].setPath("Statics3.mul", true);
    variablesMap_["uome/files/map3/statics/difs@enabled"].setBool(true, true);
    variablesMap_["uome/files/map3/statics/difs@filename-offsets"].setPath("Stadifl3.mul", true);
    variablesMap_["uome/files/map3/statics/difs@filename-idx"].setPath("Stadifi3.mul", true);
    variablesMap_["uome/files/map3/statics/difs@filename-mul"].setPath("Stadif3.mul", true);


    variablesMap_["uome/files/map4@enabled"].setBool(true, true);
    variablesMap_["uome/files/map4@filename"].setPath("Map4.mul", true);
    variablesMap_["uome/files/map4/difs@enabled"].setBool(true, true);
    variablesMap_["uome/files/map4/difs@filename-offsets"].setPath("Mapdifl4.mul", true);
    variablesMap_["uome/files/map4/difs@filename-mul"].setPath("Mapdif4.mul", true);
    variablesMap_["uome/files/map4@width"].setUint(181, true);
    variablesMap_["uome/files/map4@height"].setUint(181, true);

    variablesMap_["uome/files/map4/statics@filename-idx"].setPath("Staidx4.mul", true);
    variablesMap_["uome/files/map4/statics@filename-mul"].setPath("Statics4.mul", true);
    variablesMap_["uome/files/map4/statics/difs@enabled"].setBool(true, true);
    variablesMap_["uome/files/map4/statics/difs@filename-offsets"].setPath("Stadifl4.mul", true);
    variablesMap_["uome/files/map4/statics/difs@filename-idx"].setPath("Stadifi4.mul", true);
    variablesMap_["uome/files/map4/statics/difs@filename-mul"].setPath("Stadif4.mul", true);


    // anims
    variablesMap_["uome/files/anim0@enabled"].setBool(true, true);
    variablesMap_["uome/files/anim0@filename-idx"].setPath("Anim.idx", true);
    variablesMap_["uome/files/anim0@filename-mul"].setPath("Anim.mul", true);
    variablesMap_["uome/files/anim0@highdetail"].setUint(200, true);
    variablesMap_["uome/files/anim0@lowdetail"].setUint(200, true);

    variablesMap_["uome/files/anim1@enabled"].setBool(true, true);
    variablesMap_["uome/files/anim1@filename-idx"].setPath("Anim1.idx", true);
    variablesMap_["uome/files/anim1@filename-mul"].setPath("Anim1.mul", true);
    variablesMap_["uome/files/anim1@highdetail"].setUint(200, true);
    variablesMap_["uome/files/anim1@lowdetail"].setUint(200, true);

    variablesMap_["uome/files/anim2@enabled"].setBool(true, true);
    variablesMap_["uome/files/anim2@filename-idx"].setPath("Anim2.idx", true);
    variablesMap_["uome/files/anim2@filename-mul"].setPath("Anim2.mul", true);
    variablesMap_["uome/files/anim2@highdetail"].setUint(200, true);
    variablesMap_["uome/files/anim2@lowdetail"].setUint(200, true);

    variablesMap_["uome/files/anim3@enabled"].setBool(true, true);
    variablesMap_["uome/files/anim3@filename-idx"].setPath("Anim3.idx", true);
    variablesMap_["uome/files/anim3@filename-mul"].setPath("Anim3.mul", true);
    variablesMap_["uome/files/anim3@highdetail"].setUint(200, true);
    variablesMap_["uome/files/anim3@lowdetail"].setUint(200, true);

    variablesMap_["uome/files/anim4@enabled"].setBool(true, true);
    variablesMap_["uome/files/anim4@filename-idx"].setPath("Anim4.idx", true);
    variablesMap_["uome/files/anim4@filename-mul"].setPath("Anim4.mul", true);
    variablesMap_["uome/files/anim4@highdetail"].setUint(200, true);
    variablesMap_["uome/files/anim4@lowdetail"].setUint(200, true);

    variablesMap_["uome/files/anim5@enabled"].setBool(true, true);
    variablesMap_["uome/files/anim5@filename-idx"].setPath("Anim5.idx", true);
    variablesMap_["uome/files/anim5@filename-mul"].setPath("Anim5.mul", true);
    variablesMap_["uome/files/anim5@highdetail"].setUint(200, true);
    variablesMap_["uome/files/anim5@lowdetail"].setUint(200, true);


    // ui stuff
    variablesMap_["uome/ui/theme@name"].setPath("default", true);
    variablesMap_["uome/ui/cursor@normal-artid-start"].setUint(0x206A, true);
    variablesMap_["uome/ui/cursor@warmode-artid-start"].setUint(0x2053, true);


    // input stuff
    variablesMap_["uome/input/mouse@doubleclick-timeout-ms"].setUint(300, true);


    // shard stuff
    variablesMap_["uome/shard/account@save-password"].setBool(false, true);
}

bool Config::parseCommandLine(const std::vector<CL_String8>& args) {
    namespace po = boost::program_options;

    po::options_description consoleDesc;
    consoleDesc.add_options()
    ("help,h,?", "Receive this message")
    ("shard", po::value<std::string>(), "The shard you want to connect to. If empty, shard selection dialog is shown. Shard name can also be given without --shard prefix")
    ;

    // transform vector to default argc, argv
    int argc = args.size();
    char* argv[argc];
    for (int i = 0; i < argc; ++i) {
        argv[i] = const_cast<char*>(args[i].c_str());
    }

    po::variables_map consoleOptions;

    try {
        // parse console
        po::positional_options_description p;
        p.add("shard", 1);
        po::store(po::command_line_parser(argc, argv).options(consoleDesc).positional(p).run(), consoleOptions);

        if (consoleOptions.count("help")) {
            std::cout << consoleDesc << std::endl;
            return false;
        }

        po::notify(consoleOptions);

        //mutableVariablesMap_.insert(variablesMap_.begin(), variablesMap_.end());

    } catch (const std::exception& ex) {
        LOGARG_CRITICAL(LOGTYPE_MAIN, "Error parsing command line: %s", ex.what());
        return false;
    }

    return true;
}

bool Config::parseShardConfig(const UnicodeString& shardName) {
    //namespace po = boost::program_options;
    //po::options_description desc("All options");

    //desc.add(filesDesc_);
    //desc.add(uiDesc_);
    //desc.add(shardDesc_);

    //boost::filesystem::path cfgPath = "shards";
    //cfgPath = cfgPath / shardName / "uome.cfg";

    //if (!boost::filesystem::exists(cfgPath)) {
        //LOGARG_CRITICAL(LOGTYPE_MAIN, "Unable to open config file %s for shard %s", cfgPath.string().c_str(), shardName.c_str());
        //return false;
    //}

    //bool success = true;
    //std::ifstream stream(cfgPath.string().c_str());
    //if (!stream.is_open()) {
        //LOGARG_CRITICAL(LOGTYPE_MAIN, "Unable to open config file %s for shard %s", cfgPath.string().c_str(), shardName.c_str());
        //return false;
    //}

    //try {
        //po::store(po::parse_config_file(stream, desc), variablesMap_);

        //po::notify(variablesMap_);
    //} catch (const std::exception& ex) {
        //LOGARG_CRITICAL(LOGTYPE_MAIN, "Error parsing config file: %s", ex.what());
        //success = false;
    //}

    //stream.close();

    //mutableVariablesMap_.clear();
    //mutableVariablesMap_.insert(variablesMap_.begin(), variablesMap_.end());
    //set("shard", shardName);

    //return success;

    return true;
}

ConfigValue& Config::operator[](const UnicodeString& name) {
    return get(name);
}

ConfigValue& Config::get(const UnicodeString& name) {
    return variablesMap_[name];
}

bool Config::exists(const UnicodeString& name) const {
    return variablesMap_.count(name) > 0;
}

}
