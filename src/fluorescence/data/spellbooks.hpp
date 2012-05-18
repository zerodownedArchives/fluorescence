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


#ifndef FLUO_DATA_SPELLBOOKS_HPP
#define FLUO_DATA_SPELLBOOKS_HPP

#include <map>
#include <boost/filesystem/path.hpp>

#include <misc/string.hpp>
#include <misc/pugixml/pugixml.hpp>

namespace fluo {
namespace data {
    
struct SpellInfo {
    UnicodeString name_;
    unsigned int bitIndex_;
    UnicodeString wops_;
    UnicodeString descriptionHeader_;
    UnicodeString description_;
    UnicodeString sectionHeader_;
    unsigned int gumpId_;
    unsigned int spellId_;
};
    
struct SpellbookSectionInfo {
    UnicodeString name_;
    unsigned int byteIndex_;
    SpellInfo spells_[8];
};
    
struct SpellbookInfo {
    unsigned int packetOffset_;
    UnicodeString name_;
    UnicodeString gumpName_;
    SpellbookSectionInfo sections_[8];
};
    
    

class Spellbooks {
public:
    Spellbooks(const boost::filesystem::path& path);
    
    const SpellbookInfo* get(unsigned int packetOffset);
    
private:
    std::map<unsigned int, SpellbookInfo> spellbookInfos_;
    
    SpellbookInfo parseSpellbook(pugi::xml_node& node);
    SpellbookSectionInfo parseSection(pugi::xml_node& node);
    SpellInfo parseSpell(pugi::xml_node& node);
    
    void checkAttribute(pugi::xml_node& node, const char* name) const;
};

}
}

#endif

