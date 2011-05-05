#ifndef UOME_CONFIG_HPP
#define UOME_CONFIG_HPP

#include <vector>
#include <ClanLib/Core/Text/string8.h>
#include <map>

#include <misc/pugixml/pugixml.hpp>
#include <misc/string.hpp>
#include "configvalue.hpp"

namespace uome {
class Config {
public:
    Config();

    bool parseCommandLine(const std::vector<CL_String8>& args);
    bool parseShardConfig();

    ConfigValue& operator[](const UnicodeString& name);
    ConfigValue& get(const UnicodeString& name);

    bool exists(const UnicodeString& name) const;

    bool save(const boost::filesystem::path& path, bool includeDefaultValues) const;
    void dumpMap() const;

private:
    std::map<UnicodeString, ConfigValue> variablesMap_;

    Config(const Config& copy) { }
    Config& operator=(const Config& copy) { return *this; }

    bool parseNode(const pugi::xml_node& node);
    pugi::xml_node buildXmlNode(pugi::xml_node& parent, const UnicodeString& path) const;
};

}

#endif
