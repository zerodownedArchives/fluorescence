#ifndef UOME_CONFIG_HPP
#define UOME_CONFIG_HPP

#include <vector>

#include <boost/program_options.hpp>
#include <ClanLib/Core/Text/string8.h>

#define UOME_DEFAULT_CONFIG_FILE_NAME "uome.cfg"

namespace uome {
class Config {
public:
    static bool getVariablesMap(const std::vector<CL_String8>& args, boost::program_options::variables_map& map);
};
}

#endif
