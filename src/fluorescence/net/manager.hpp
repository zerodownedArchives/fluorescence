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
