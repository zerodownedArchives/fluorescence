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


#ifndef FLUO_CLIENT_HPP
#define FLUO_CLIENT_HPP

#include <vector>
#include <ClanLib/Core/Text/string8.h>

#include <boost/program_options.hpp>

#include <misc/config.hpp>

namespace fluo {

namespace world {
    class Sector;
}

namespace ui {
    class GumpMenu;
}

class Client {
public:
    enum {
        STATE_SHARD_SELECTION,
        STATE_PRE_LOGIN,
        STATE_LOGIN,
        STATE_PLAYING,
        STATE_SHUTDOWN,
    };

    Client();

    static Client* getSingleton();

    static int sMain(const std::vector<CL_String8>& args); ///< static helper to call main
    int main(const std::vector<CL_String8>& args);

    Config& getConfig();

    void shutdown();
    void setState(unsigned int state);
    unsigned int getState();
    void loginComplete(); /// < called when the game window should be displayed => setState(playing)
    void disconnect();

    // gump callbacks
    bool disconnect(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);
    bool shutdown(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);
    bool selectShard(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);
    bool selectCharacter(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);
    
    timeval getElapsedTime() const;

private:
    static Client* singleton_;

    Config config_;

    void cleanUp();
    void saveConfig();

    bool initBase(const std::vector<CL_String8>& args);
    bool initFull();

    float calculateFps(unsigned int elapsedMillis);

    unsigned int state_;
    unsigned int requestedState_;

    bool handleStateChange();

    void doStateShardSelection();
    void doStatePreLogin();
    void doStateLogin();
    void doStatePlaying(unsigned int elapsedMillis);
    
    timeval startTime_;
};

}

#endif
