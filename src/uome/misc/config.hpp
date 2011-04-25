#ifndef UOME_CONFIG_HPP
#define UOME_CONFIG_HPP

#include <vector>

#include <boost/program_options.hpp>
#include <ClanLib/Core/Text/string8.h>

#include <iostream>
#include <stdio.h>

namespace uome {
class Config {
public:
    Config();

    bool parseCommandLine(const std::vector<CL_String8>& args);
    bool parseShardConfig(const std::string& shardName);

    boost::program_options::variable_value& operator[](const std::string& name);
    boost::program_options::variable_value& get(const std::string& name);

    unsigned int count(const std::string& name) const;

    template<typename T>
    void set(const std::string& name, const T& value) {
        mutableVariablesMap_[name].value() = value;
    }

private:
    boost::program_options::options_description consoleDesc_;
    boost::program_options::options_description filesDesc_;
    boost::program_options::options_description uiDesc_;

    boost::program_options::variables_map variablesMap_;

    std::map<std::string, boost::program_options::variable_value> mutableVariablesMap_;
};

}

#endif
