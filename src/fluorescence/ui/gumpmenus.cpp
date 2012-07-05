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



#include "gumpmenus.hpp"

#include <sstream>
#include <list>
#include <vector>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <client.hpp>
#include <misc/log.hpp>
#include <misc/config.hpp>
#include <misc/exception.hpp>

#include <net/packets/serverlist.hpp>
#include <net/packets/characterlist.hpp>
#include <net/packets/bf/opencontextmenu.hpp>

#include <data/manager.hpp>
#include <data/clilocloader.hpp>
#include <data/spellbooks.hpp>
#include <data/skillsloader.hpp>

#include <world/dynamicitem.hpp>
#include <world/mobile.hpp>

#include "gumpmenu.hpp"
#include "xmlparser.hpp"
#include "components/lineedit.hpp"
#include "components/label.hpp"
#include "components/basebutton.hpp"

namespace fluo {
namespace ui {

GumpMenu* GumpMenus::openMessageBox(const UnicodeString& message) {
    GumpMenu* menu = XmlParser::fromXmlFile("messagebox");
    if (menu) {
        menu->setComponentText<components::Label>("messagetext", message);
    }

    LOG_INFO << "MessageBox: " << message << std::endl;

    return menu;
}

GumpMenu* GumpMenus::openYesNoBox(const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    if (parameterCount < 2) {
        LOG_ERROR << "No text or action given for YesNoBox" << std::endl;
        return nullptr;
    }

    GumpMenu* menu = XmlParser::fromXmlFile("yesnobox");
    if (menu) {
        menu->setComponentText<components::Label>("messagetext", parameters[0]);

        components::BaseButton* yesButton = dynamic_cast<components::BaseButton*>(menu->get_named_item("yes"));
        if (yesButton) {
            yesButton->setLocalButton(parameters[1]);

            for (unsigned int i = 2; i < parameterCount; ++i) {
                yesButton->setParameter(parameters[i], i-2);
            }
        } else {
            LOG_ERROR << "Unable to find yes button in yesnobox" << std::endl;
        }
    }

    LOG_INFO << "YesNoBox: " << parameters[0] << std::endl;

    return menu;
}

GumpMenu* GumpMenus::openLoginGump() {
    GumpMenu* menu = XmlParser::fromXmlFile("login");

    if (menu) {
        Config& config = Client::getSingleton()->getConfig();
        menu->setComponentTextFromConfig<components::LineEdit>("loginhost", "/fluo/shard/address@host", config);

        if (config.exists("/fluo/shard/address@port")) {
            UnicodeString portStr = StringConverter::fromNumber(config["/fluo/shard/address@port"].asInt());

            menu->setComponentText<components::LineEdit>("loginport", portStr);
        }

        menu->setComponentTextFromConfig<components::LineEdit>("loginaccount", "/fluo/shard/account@name", config);
        menu->setComponentTextFromConfig<components::LineEdit>("loginpassword", "/fluo/shard/account@password", config);

    }

    return menu;
}

GumpMenu* GumpMenus::openShardSelectionGump() {
    namespace bfs = boost::filesystem;

    bfs::path path("shards");

    if (!bfs::exists(path) || !bfs::is_directory(path)) {
        LOG_EMERGENCY << "Unable to list shards directory" << std::endl;
        return NULL;
    }

    std::vector<UnicodeString> nameList;

    bfs::directory_iterator nameIter(path);
    bfs::directory_iterator nameEnd;

    for (; nameIter != nameEnd; ++nameIter) {
        if (bfs::is_directory(nameIter->status())) {
            nameList.push_back(StringConverter::fromUtf8(nameIter->path().filename()));
        }
    }

    XmlParser::RepeatContext context;
    context.repeatCount_ = nameList.size();
    context.keywordReplacements_["shardname"] = nameList;
    context.keywordReplacements_["shardname"] = nameList;

    XmlParser::addRepeatContext("shardlist", context);
    GumpMenu* menu = XmlParser::fromXmlFile("shardselection");
    XmlParser::removeRepeatContext("shardlist");

    return menu;
}

GumpMenu* GumpMenus::openServerListGump(const net::packets::ServerList* list) {
    std::vector<UnicodeString> nameList;
    std::vector<UnicodeString> indexList;

    std::list<net::packets::ServerList::ServerListEntry>::const_iterator entryIter = list->listEntries_.begin();
    std::list<net::packets::ServerList::ServerListEntry>::const_iterator entryEnd = list->listEntries_.end();


    for (; entryIter != entryEnd; ++entryIter) {
        nameList.push_back(entryIter->name_);

        indexList.push_back(StringConverter::fromNumber(entryIter->index_));
    }

    XmlParser::RepeatContext context;
    context.repeatCount_ = nameList.size();
    context.keywordReplacements_["serverindex"] = indexList;
    context.keywordReplacements_["servername"] = nameList;

    XmlParser::addRepeatContext("serverlist", context);
    GumpMenu* menu = XmlParser::fromXmlFile("serverlist");
    XmlParser::removeRepeatContext("serverlist");

    return menu;
}

GumpMenu* GumpMenus::openCharacterListGump(unsigned int charCount, const UnicodeString* charNames, const UnicodeString* charPasswords) {
    std::vector<UnicodeString> nameList;
    std::vector<UnicodeString> indexList;
    std::vector<UnicodeString> passwordList;

    for (unsigned int i = 0; i < charCount; ++i) {
        nameList.push_back(charNames[i]);
        indexList.push_back(StringConverter::fromNumber(i));
        passwordList.push_back(charPasswords[i]);
    }

    XmlParser::RepeatContext context;
    context.repeatCount_ = nameList.size();
    context.keywordReplacements_["characterindex"] = indexList;
    context.keywordReplacements_["charactername"] = nameList;
    context.keywordReplacements_["characterpassword"] = nameList;

    XmlParser::addRepeatContext("characterlist", context);
    GumpMenu* menu = XmlParser::fromXmlFile("characterlist");
    XmlParser::removeRepeatContext("characterlist");

    return menu;
}

GumpMenu* GumpMenus::openContextMenu(const net::packets::bf::OpenContextMenu* pkt) {
    std::vector<UnicodeString> nameList;
    std::vector<UnicodeString> indexList;
    std::vector<UnicodeString> serialList;

    std::list<net::packets::bf::ContextMenuEntry>::const_iterator iter = pkt->entries_.begin();
    std::list<net::packets::bf::ContextMenuEntry>::const_iterator end = pkt->entries_.end();

    UnicodeString serialString = StringConverter::fromNumber(pkt->serial_);
    for (; iter != end; ++iter) {
        indexList.push_back(StringConverter::fromNumber(iter->entryId_));
        nameList.push_back(data::Manager::getClilocLoader()->get(iter->clilocId_));
        serialList.push_back(serialString);
    }

    XmlParser::RepeatContext context;
    context.repeatCount_ = nameList.size();
    context.keywordReplacements_["entrytext"] = nameList;
    context.keywordReplacements_["serial"] = serialList;
    context.keywordReplacements_["entryindex"] = indexList;

    XmlParser::addRepeatContext("contextmenu", context);
    GumpMenu* menu = XmlParser::fromXmlFile("contextmenu");
    XmlParser::removeRepeatContext("contextmenu");

    return menu;
}

GumpMenu* GumpMenus::openSpellbook(const world::DynamicItem* itm) {
    const data::SpellbookInfo* book = data::Manager::getSpellbookInfo(itm->getSpellbookScrollOffset());
    if (!book) {
        return nullptr;
    }

    XmlParser::RepeatContext indexContexts[8];
    std::vector<UnicodeString> circleIndexNamesTmp;
    std::vector<UnicodeString> circleIndexPagesTmp;

    std::vector<UnicodeString> allSpellsPreviousPage;
    std::vector<UnicodeString> allSpellsCurrentPage;
    std::vector<UnicodeString> allSpellsNextPage;
    std::vector<UnicodeString> allSpellsSections;
    std::vector<UnicodeString> allSpellsNames;
    std::vector<UnicodeString> allSpellsIcons;
    std::vector<UnicodeString> allSpellsDescriptionHeaders;
    std::vector<UnicodeString> allSpellsDescriptions;
    std::vector<UnicodeString> allSpellsIds;

    unsigned int spellCount = 0;
    unsigned int curPage = 4; // index spreads over 4 pages

    for (unsigned int i = 0; i < 8; ++i) {
        circleIndexNamesTmp.clear();
        circleIndexPagesTmp.clear();

        uint8_t spellBits = itm->getSpellbookSpellBits(i);
        for (unsigned int j = 0; j < 8; ++j) {
            uint8_t test = (1 << j);
            if (test & spellBits) {
                // spell available
                ++spellCount;
                if (spellCount % 2 == 1) {
                    ++curPage;
                }

                circleIndexNamesTmp.push_back(book->sections_[i].spells_[j].name_);
                circleIndexPagesTmp.push_back(StringConverter::fromNumber(curPage));


                allSpellsNames.push_back(book->sections_[i].spells_[j].name_);
                allSpellsSections.push_back(book->sections_[i].name_);
                allSpellsPreviousPage.push_back(StringConverter::fromNumber(curPage - 1));
                allSpellsCurrentPage.push_back(StringConverter::fromNumber(curPage));
                allSpellsNextPage.push_back(StringConverter::fromNumber(curPage + 1));
                allSpellsIcons.push_back(StringConverter::fromNumber(book->sections_[i].spells_[j].gumpId_));
                allSpellsDescriptionHeaders.push_back(book->sections_[i].spells_[j].descriptionHeader_);
                allSpellsDescriptions.push_back(book->sections_[i].spells_[j].description_);
                allSpellsIds.push_back(StringConverter::fromNumber(book->sections_[i].spells_[j].spellId_));
            }
        }

        indexContexts[i].repeatCount_ = circleIndexNamesTmp.size();
        indexContexts[i].keywordReplacements_["spellname"] = circleIndexNamesTmp;
        indexContexts[i].keywordReplacements_["spellpage"] = circleIndexPagesTmp;

        XmlParser::addRepeatContext(book->sections_[i].name_, indexContexts[i]);
    }

    XmlParser::RepeatContext allSpellsContext;
    allSpellsContext.repeatCount_ = allSpellsNames.size();
    allSpellsContext.keywordReplacements_["previouspage"] = allSpellsPreviousPage;
    allSpellsContext.keywordReplacements_["currentpage"] = allSpellsCurrentPage;
    allSpellsContext.keywordReplacements_["nextpage"] = allSpellsNextPage;
    allSpellsContext.keywordReplacements_["sectionname"] = allSpellsSections;
    allSpellsContext.keywordReplacements_["spellname"] = allSpellsNames;
    allSpellsContext.keywordReplacements_["description"] = allSpellsDescriptions;
    allSpellsContext.keywordReplacements_["descriptionheader"] = allSpellsDescriptionHeaders;
    allSpellsContext.keywordReplacements_["gumpid"] = allSpellsIcons;
    allSpellsContext.keywordReplacements_["spellid"] = allSpellsIds;

    XmlParser::addRepeatContext("allspells", allSpellsContext);

    GumpMenu* menu = XmlParser::fromXmlFile(book->gumpName_);
    XmlParser::clearRepeatContexts();

    return menu;
}

GumpMenu* GumpMenus::openSkills(const world::Mobile* mob) {
    std::vector<UnicodeString> nameList;
    std::vector<UnicodeString> valueList;
    std::vector<UnicodeString> baseList;
    std::vector<UnicodeString> capList;
    std::vector<UnicodeString> usableList;

    unsigned int skillCount = data::Manager::getSkillsLoader()->getSkillCount();

    for (unsigned int i = 0; i < skillCount; ++i) {
        const data::SkillInfo* curInfo = data::Manager::getSkillsLoader()->getSkillInfo(i);
        nameList.push_back(curInfo->name_);

        UnicodeString propNameBase("skills.");
        propNameBase += curInfo->name_;

        UnicodeString propNameCur(propNameBase);
        propNameCur += ".value";
        valueList.push_back(propNameCur);

        propNameCur = propNameBase;
        propNameCur += ".base";
        baseList.push_back(propNameCur);

        propNameCur = propNameBase;
        propNameCur += ".cap";
        capList.push_back(propNameCur);

        if (curInfo->isUsable_) {
            usableList.push_back("1");
        } else {
            usableList.push_back("0");
        }
    }

    XmlParser::RepeatContext context;
    context.repeatCount_ = nameList.size();
    context.keywordReplacements_["skillname"] = nameList;
    context.keywordReplacements_["skillvalue"] = valueList;
    context.keywordReplacements_["skillbase"] = baseList;
    context.keywordReplacements_["skillcap"] = capList;
    context.keywordReplacements_["skillusable"] = usableList;

    XmlParser::addRepeatContext("skills", context);
    GumpMenu* menu = XmlParser::fromXmlFile("skills");
    XmlParser::removeRepeatContext("contextmenu");

    return menu;
}

}
}
