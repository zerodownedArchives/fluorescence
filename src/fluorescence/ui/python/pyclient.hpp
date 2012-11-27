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


#ifndef FLUO_UI_PYTHON_PYCLIENT_HPP
#define FLUO_UI_PYTHON_PYCLIENT_HPP

#include <misc/string.hpp>

namespace fluo {
namespace ui {
namespace python {

class PyClient {
public:
    static void setShard(const UnicodeString& name);
    static bool createShard(const UnicodeString& name, const UnicodeString& path, bool highSeas);

    static bool connect(const UnicodeString& host, unsigned int port, const UnicodeString& account, const UnicodeString& password);
    static void disconnect();
    static void selectServer(unsigned int index);
    static void selectCharacter(unsigned int index, const UnicodeString& name, const UnicodeString& password);

    static UnicodeString getConfig(const UnicodeString& key);
    static void shutdown();

    static void messagebox(const UnicodeString& msg);
    static void openGump(const UnicodeString& name);

    static void handleTextInput(const UnicodeString& text);

    static void sendHelpRequest();

    static void useSkill(unsigned int skillId);
    static void castSpell(unsigned int spellId);

    static void objectPickerResponse(unsigned int serial, unsigned int menuId, unsigned int answerId, unsigned int artId, unsigned int hue);
    static void contextMenuReply(unsigned int serial, unsigned int entryId);
};

}
}
}

#endif

