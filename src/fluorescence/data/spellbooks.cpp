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

#include "spellbooks.hpp"

#include <misc/xmlloadexception.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace data {
    
Spellbooks::Spellbooks(const boost::filesystem::path& path) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.string().c_str());

    if (result) {
        pugi::xml_node spellbooksNode = doc.document_element();
        pugi::xml_node spellbookIter = spellbooksNode.first_child();
        while (spellbookIter) {
            SpellbookInfo curInfo = parseSpellbook(spellbookIter);
            spellbookInfos_[curInfo.packetOffset_] = curInfo;
            spellbookIter = spellbookIter.next_sibling();
        }
    } else {
        LOG_ERROR << "Error parsing xml spellbook definitions at offset " << result.offset << ": " << result.description() << std::endl;
        throw XmlLoadException("Unable to load xml spellbook definitions");
    }
}

const SpellbookInfo* Spellbooks::get(unsigned int packetOffset) {
    std::map<unsigned int, SpellbookInfo>::const_iterator iter = spellbookInfos_.find(packetOffset);
    if (iter != spellbookInfos_.end()) {
        return &iter->second;
    } else {
        LOG_ERROR << "Trying to open spellbook for unknown packet offset: " << packetOffset << std::endl;
        return nullptr;
    }
}

SpellbookInfo Spellbooks::parseSpellbook(pugi::xml_node& node) {
    SpellbookInfo curInfo;
    checkAttribute(node, "offset");
    checkAttribute(node, "name");
    checkAttribute(node, "gump");
    curInfo.packetOffset_ = node.attribute("offset").as_uint();
    curInfo.name_ = node.attribute("name").value();
    curInfo.gumpName_ = node.attribute("gump").value();
    
    pugi::xml_node sectionIter = node.first_child();
    while (sectionIter) {
        SpellbookSectionInfo curSection = parseSection(sectionIter);
        for (unsigned int i = 0; i < 8; ++i) {
            curSection.spells_[i].spellId_ += curInfo.packetOffset_;
        }
        
        curInfo.sections_[curSection.byteIndex_] = curSection;
        sectionIter = sectionIter.next_sibling();
    }
    
    return curInfo;
}

SpellbookSectionInfo Spellbooks::parseSection(pugi::xml_node& node) {
    SpellbookSectionInfo curInfo;
    checkAttribute(node, "byte");
    checkAttribute(node, "name");
    curInfo.byteIndex_ = node.attribute("byte").as_uint();
    curInfo.name_ = node.attribute("name").value();
    
    if (curInfo.byteIndex_ > 7) {
        LOG_ERROR << "Byte index of spellbook section must not be > 7" << std::endl;
        throw XmlLoadException("Byte index of spellbook section must not be > 7");
    }
    
    pugi::xml_node spellIter = node.first_child();
    while (spellIter) {
        SpellInfo curSpell = parseSpell(spellIter);
        curSpell.sectionHeader_ = curInfo.name_;
        curSpell.spellId_ += curInfo.byteIndex_ * 8;
        curInfo.spells_[curSpell.bitIndex_] = curSpell;
        spellIter = spellIter.next_sibling();
    }
    
    return curInfo;
}

SpellInfo Spellbooks::parseSpell(pugi::xml_node& node) {
    SpellInfo curInfo;
    checkAttribute(node, "bit");
    checkAttribute(node, "name");
    checkAttribute(node, "wops");
    checkAttribute(node, "description-header");
    checkAttribute(node, "description");
    checkAttribute(node, "gumpid");
    curInfo.bitIndex_ = node.attribute("bit").as_uint();
    curInfo.name_ = node.attribute("name").value();
    curInfo.wops_ = node.attribute("wops").value();
    curInfo.descriptionHeader_ = node.attribute("description-header").value();
    curInfo.description_ = node.attribute("description").value();
    curInfo.gumpId_ = node.attribute("gumpid").as_uint();
    curInfo.spellId_ = curInfo.bitIndex_; // section and book offset are added in parseSection and parseSpellbook
    
    if (curInfo.bitIndex_ > 7) {
        LOG_ERROR << "Bit index of spell must not be > 7" << std::endl;
        throw XmlLoadException("Bit index of spell must not be > 7");
    }
    
    return curInfo;
}

void Spellbooks::checkAttribute(pugi::xml_node& node, const char* name) const {
    if (!node.attribute(name)) {
        std::string str("Missing attribute ");
        str += name;
        str += " in node ";
        str += node.name();
        throw XmlLoadException(str);
    }
}

}
}

