#ifndef UOME_CLIENT_HPP
#define UOME_CLIENT_HPP

#include <vector>
#include <ClanLib/Core/Text/string8.h>

#include <boost/program_options.hpp>

#include <misc/config.hpp>

namespace uome {

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

    // gump callbacks
    bool disconnect(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);
    bool shutdown(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);
    bool selectShard(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);
    bool selectCharacter(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);

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
};

}

#endif
