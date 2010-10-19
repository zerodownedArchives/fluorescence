#ifndef UOME_CONFIG_HPP
#define UOME_CONFIG_HPP

#include <vector>
#include <iostream>
#include <fstream>

#include <ClanLib/Core/Text/string8.h>

#include <boost/program_options.hpp>

#include <logger.hpp>


#define UOME_DEFAULT_CONFIG_FILE_NAME "uome.cfg"

namespace uome {
class Config {
public:
    static bool getVariablesMap(const std::vector<CL_String8>& args, boost::program_options::variables_map& map) {

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

};
}

#endif
