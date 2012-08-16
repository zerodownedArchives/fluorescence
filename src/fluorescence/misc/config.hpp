/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef FLUO_CONFIG_HPP
#define FLUO_CONFIG_HPP

#include <vector>
#include <ClanLib/Core/Text/string8.h>
#include <map>
#include <boost/filesystem/path.hpp>

#include <misc/pugixml/pugixml.hpp>
#include <misc/string.hpp>
#include "variable.hpp"

namespace fluo {
class Config {
public:
    Config();

    void initDefaults();

    bool parseCommandLine(const std::vector<CL_String8>& args);
    bool parseShardConfig();

    Variable& operator[](const UnicodeString& name);
    Variable& get(const UnicodeString& name);

    bool exists(const UnicodeString& name) const;

    bool save(const boost::filesystem::path& path, bool includeDefaultValues);
    void dumpMap() const;

    const UnicodeString& getShardName() const;
    boost::filesystem::path getShardPath() const;
    void setShardName(const UnicodeString& name);

    bool isLoaded() const;

private:
    std::map<UnicodeString, Variable> variablesMap_;
    UnicodeString shardName_;

    Config(const Config& copy) { }
    Config& operator=(const Config& copy) { return *this; }

    bool parseNode(const pugi::xml_node& node);
    pugi::xml_node buildXmlNode(pugi::xml_node& parent, const UnicodeString& path) const;
};

}

#endif
