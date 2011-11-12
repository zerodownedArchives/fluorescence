#ifndef FLUO_NET_MANAGER_HPP
#define FLUO_NET_MANAGER_HPP

#include <boost/shared_ptr.hpp>

#include <misc/config.hpp>

#include "packet.hpp"
#include "socket.hpp"

namespace fluo {
namespace ui {
    class GumpMenu;
}

namespace net {
class WalkManager;

namespace packets {
    class ServerRedirect;
}

class Manager {
public:
    static bool create(const Config& config);
    static Manager* getSingleton();
    static void destroy();

    void step();

    static boost::shared_ptr<Packet> createPacket(uint8_t id);
    static boost::shared_ptr<WalkManager> getWalkManager();

    bool connect(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);
    void disconnect();

    bool selectServer(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);

    void handleServerRedirect(const packets::ServerRedirect* packet);

    template<class P>
    bool send(const P& packet) {
        return socket_.write(packet);
    }

    uint32_t getSeed() const;

private:
    static Manager* singleton_;
    Manager(const Config& config);
    Manager(const Manager& copy) { }
    Manager& operator=(const Manager& copy) { return *this; }
    ~Manager();

    Socket socket_;

    boost::shared_ptr<WalkManager> walkManager_;
};

}
}

#endif
