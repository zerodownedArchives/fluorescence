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
    namespace components {
        class LocalButton;
    }
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
    void loginComplete(); /// < called when the game window should be displayed

    // gump callbacks
    bool disconnect(ui::GumpMenu* menu, ui::components::LocalButton* button);
    bool shutdown(ui::GumpMenu* menu, ui::components::LocalButton* button);
    bool selectShard(ui::GumpMenu* menu, ui::components::LocalButton* button);
    bool selectCharacter(ui::GumpMenu* menu, ui::components::LocalButton* button);

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
