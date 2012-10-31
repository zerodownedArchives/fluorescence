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
class WalkPacketManager;

namespace packets {
    class ServerRedirect;
}

struct ProtocolVersion {
enum {
    PRE_HS = 0,
    HS = 2,
    HS_7013 = 3,
};
};

class Manager {
public:
    static bool create(Config& config);
    static Manager* getSingleton();
    static void destroy();

    void step();

    static boost::shared_ptr<Packet> createPacket(uint8_t id);
    static boost::shared_ptr<WalkPacketManager> getWalkPacketManager();

    bool connect(const UnicodeString& host, unsigned int port, const UnicodeString& account, const UnicodeString& password);
    void disconnect();

    bool selectServer(ui::GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);

    void handleServerRedirect(const packets::ServerRedirect* packet);

    template<class P>
    bool send(const P& packet) {
        lastSendTime_ = time(NULL);
        return socket_.write(packet);
    }

    uint32_t getSeed() const;

    unsigned int getProtocolVersion() const;

private:
    static Manager* singleton_;
    Manager(Config& config);
    Manager(const Manager& copy) { }
    Manager& operator=(const Manager& copy) { return *this; }
    ~Manager();

    Socket socket_;

    boost::shared_ptr<WalkPacketManager> walkPacketManager_;

    unsigned int protocolVersion_;

    time_t lastSendTime_;
};

}
}

#endif
