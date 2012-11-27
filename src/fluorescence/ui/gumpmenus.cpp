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

#include <net/packets/7c_objectpicker.hpp>
#include <net/packets/a8_serverlist.hpp>
#include <net/packets/a9_characterlist.hpp>
#include <net/packets/bf/14_opencontextmenu.hpp>

#include <data/manager.hpp>
#include <data/clilocloader.hpp>
#include <data/spellbooks.hpp>
#include <data/skillsloader.hpp>

#include <world/dynamicitem.hpp>
#include <world/mobile.hpp>

#include "manager.hpp"
#include "gumpmenu.hpp"
#include "xmlloader.hpp"
#include "components/lineedit.hpp"
#include "components/label.hpp"

namespace fluo {
namespace ui {

void GumpMenus::openMessageBox(const UnicodeString& message) {
    boost::python::dict args;
    args["message"] = message;
    ui::Manager::getSingleton()->openPythonGump("messagebox", args);
    LOG_INFO << "MessageBox: " << message << std::endl;
}

void GumpMenus::openShardList() {
    namespace bfs = boost::filesystem;

    bfs::path path("shards");

    if (!bfs::exists(path) || !bfs::is_directory(path)) {
        LOG_EMERGENCY << "Unable to list shards directory" << std::endl;
        openMessageBox("Unable to list shards directory");
        return;
    }

    boost::python::list nameList;

    bfs::directory_iterator nameIter(path);
    bfs::directory_iterator nameEnd;
    for (; nameIter != nameEnd; ++nameIter) {
        if (bfs::is_directory(nameIter->status()) && nameIter->leaf() != ".svn") {
            nameList.append(StringConverter::fromUtf8(nameIter->path().filename()));
        }
    }

    nameList.sort();

    boost::python::dict args;
    args["shardlist"] = nameList;

    ui::Manager::getSingleton()->openPythonGump("shardlist", args);
}

void GumpMenus::openServerList(const net::packets::ServerList* list) {
    boost::python::list serverList;

    std::list<net::packets::ServerList::ServerListEntry>::const_iterator entryIter = list->listEntries_.begin();
    std::list<net::packets::ServerList::ServerListEntry>::const_iterator entryEnd = list->listEntries_.end();

    for (; entryIter != entryEnd; ++entryIter) {
        serverList.append(boost::python::make_tuple(entryIter->index_, entryIter->name_));
    }

    boost::python::dict args;
    args["serverlist"] = serverList;
    ui::Manager::getSingleton()->openPythonGump("serverlist", args);
}

void GumpMenus::openCharacterList(unsigned int charCount, const UnicodeString* charNames, const UnicodeString* charPasswords) {
    boost::python::list charList;

    for (unsigned int i = 0; i < charCount; ++i) {
        charList.append(
            boost::python::make_tuple(i, charNames[i], charPasswords[i])
        );
    }

    boost::python::dict args;
    args["characterlist"] = charList;
    ui::Manager::getSingleton()->openPythonGump("characterlist", args);
}

void GumpMenus::openObjectPicker(const net::packets::ObjectPicker* pkt) {
    boost::python::list answerList;

    std::vector<net::packets::ObjectPickerItem>::const_iterator iter = pkt->items_.begin();
    std::vector<net::packets::ObjectPickerItem>::const_iterator end = pkt->items_.end();
    for (; iter != end; ++iter) {
        answerList.append(boost::python::make_tuple(iter->answerId_, iter->text_, iter->artId_, iter->hue_));
    }

    boost::python::dict args;
    args["answers"] = answerList;
    args["question"] = pkt->title_;
    args["menuid"] = pkt->menuId_;
    args["serial"] = pkt->serial_;
    ui::Manager::getSingleton()->openPythonGump("objectpicker", args);
}

void GumpMenus::openContextMenu(const net::packets::bf::OpenContextMenu* pkt) {
    boost::python::list entries;

    std::list<net::packets::bf::ContextMenuEntry>::const_iterator iter = pkt->entries_.begin();
    std::list<net::packets::bf::ContextMenuEntry>::const_iterator end = pkt->entries_.end();
    for (; iter != end; ++iter) {
        entries.append(boost::python::make_tuple(iter->entryId_, iter->clilocId_));
    }

    boost::python::dict args;
    args["serial"] = pkt->serial_;
    args["entries"] = entries;
    ui::Manager::getSingleton()->openPythonGump("contextmenu", args);
}

void GumpMenus::openSpellbook(const boost::shared_ptr<world::DynamicItem>& itm) {
    const data::SpellbookInfo* book = data::Manager::getSpellbookInfo(itm->getSpellbookScrollOffset());
    if (!book) {
        LOG_WARN << "Unable to find spellbook data for spellbook " << itm->getSpellbookScrollOffset() << std::endl;
        return;
    }

    namespace bpy = boost::python;
    bpy::list circleList;

    unsigned int spellCount = 0;
    unsigned int curPage = 4; // index spreads over 4 pages

    for (unsigned int i = 0; i < 8; ++i) {
        bpy::list curSpellList;

        uint8_t spellBits = itm->getSpellbookSpellBits(i);
        for (unsigned int j = 0; j < 8; ++j) {
            uint8_t test = (1 << j);
            if (test & spellBits) {
                // spell available
                ++spellCount;
                if (spellCount % 2 == 1) {
                    ++curPage;
                }

                const data::SpellInfo& spell = book->sections_[i].spells_[j];
                curSpellList.append(bpy::make_tuple(
                    spell.spellId_,
                    spell.name_,
                    spell.wops_,
                    spell.descriptionHeader_,
                    spell.description_,
                    spell.gumpId_
                ));
            }
        }

        bpy::dict curCircle;
        curCircle["spells"] = curSpellList;
        curCircle["id"] = i+1;
        curCircle["name"] = book->sections_[i].name_;

        circleList.append(curCircle);
    }

    bpy::dict args;
    args["circles"] = circleList;
    args["item"] = itm;
    ui::Manager::getSingleton()->openPythonGump(book->gumpName_, args);
}


// rework to python ui below this point

GumpMenu* GumpMenus::openYesNoBox(const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters) {
    if (parameterCount < 2) {
        LOG_ERROR << "No text or action given for YesNoBox" << std::endl;
        return nullptr;
    }

    GumpMenu* menu = XmlLoader::fromXmlFile("yesnobox");
    if (menu) {
        // TODO: gui rework
        //menu->setComponentText<components::Label>("messagetext", parameters[0]);

        //components::BaseButton* yesButton = dynamic_cast<components::BaseButton*>(menu->get_named_item("yes"));
        //if (yesButton) {
            //yesButton->setLocalButton(parameters[1]);

            //for (unsigned int i = 2; i < parameterCount; ++i) {
                //yesButton->setParameter(parameters[i], i-2);
            //}
        //} else {
            //LOG_ERROR << "Unable to find yes button in yesnobox" << std::endl;
        //}
    }

    LOG_INFO << "YesNoBox: " << parameters[0] << std::endl;

    return menu;
}


}
}
