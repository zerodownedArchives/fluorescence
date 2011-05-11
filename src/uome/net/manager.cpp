
#include "manager.hpp"

#include <client.hpp>
#include <misc/log.hpp>
#include <misc/exception.hpp>
#include <ui/manager.hpp>
#include <ui/gumpmenu.hpp>
#include <ui/gumpmenus.hpp>
#include <ui/components/lineedit.hpp>
#include <ui/components/localbutton.hpp>

#include "packetlist.hpp"
#include "twofishencryption.hpp"

namespace uome {
namespace net {

Manager* Manager::singleton_ = 0;

bool Manager::create(const Config& config) {
    if (!singleton_) {
        try {
            singleton_ = new Manager(config);
        } catch (const std::exception& ex) {
            LOG_EMERGENCY << "Error initializing world::Manager: " << ex.what() << std::endl;
            return false;
        }
    }

    return true;
}

Manager* Manager::getSingleton() {
    if (!singleton_) {
        throw uome::Exception("uome::net::Manager Singleton not created yet");
    }

    return singleton_;
}

void Manager::destroy() {
    if (singleton_) {
        delete singleton_;
        singleton_ = NULL;
    }
}

Manager::Manager(const Config& config) {
}

Manager::~Manager() {
    LOG_INFO << "net::Manager shutdown" << std::endl;
}

void Manager::step() {
    if (socket_.isConnected()) {
        // handle received packets
        boost::shared_ptr<Packet> packet = socket_.getNextPacket();
        while (packet) {
            packet->onReceive();
            packet = socket_.getNextPacket();
        }

        // send packets to server
        socket_.sendAll();
    }
}

boost::shared_ptr<Packet> Manager::createPacket(uint8_t id) {
    boost::shared_ptr<Packet> ret;

    switch (id) {
        case 0xA8: ret.reset(new packets::ServerList()); break;
        case 0x8C: ret.reset(new packets::ServerRedirect()); break;
        case 0xA9: ret.reset(new packets::CharacterList()); break;
        default: ret.reset(new packets::Unknown(id)); break;
    }

    return ret;
}

bool Manager::connect(ui::GumpMenu* menu, ui::components::LocalButton* button) {
    UnicodeString host;
    if (!menu->getComponentText<ui::components::LineEdit>("loginhost", host)) {
        ui::GumpMenus::openMessageBox("Unable to find input field for the server host. Did you change the login gump template?");
        return false;
    }

    ui::components::LineEdit* line = dynamic_cast<ui::components::LineEdit*>(menu->get_named_item("loginport"));
    if (!line) {
        ui::GumpMenus::openMessageBox("Unable to find input field for the server port. Did you change the login gump template?");
        return false;
    }

    int port = line->get_text_int();
    if (port <= 0) {
        ui::GumpMenus::openMessageBox("Unable to parse port number");
        return false;
    }


    UnicodeString accName;
    if (!menu->getComponentText<ui::components::LineEdit>("loginaccount", accName)) {
        ui::GumpMenus::openMessageBox("Unable to find input field for the account name. Did you change the login gump template?");
        return false;
    }

    UnicodeString accPw;
    if (!menu->getComponentText<ui::components::LineEdit>("loginpassword", accPw)) {
        ui::GumpMenus::openMessageBox("Unable to find input field for the password. Did you change the login gump template?");
        return false;
    }


    if (socket_.connect(host, port)) {
        Client* clientSing = Client::getSingleton();
        clientSing->setState(Client::STATE_LOGIN);

        clientSing->getConfig()["shard.account"].setString(accName);
        clientSing->getConfig()["shard.password"].setString(accPw);

        socket_.writeSeed(0);
        // send packet
        packets::LoginRequest req(accName, accPw);
        socket_.write(req);

        return true;
    } else {
        ui::GumpMenus::openMessageBox("Could not connect to server");
        return false;
    }
}

void Manager::disconnect() {
    socket_.close();
}

bool Manager::selectServer(ui::GumpMenu* menu, ui::components::LocalButton* button) {
    unsigned int index = button->getParameterInt();

    packets::GameServerSelect pkt(index);
    socket_.write(pkt);

    return true;
}

void Manager::handleServerRedirect(const packets::ServerRedirect* packet) {
    socket_.close();

    // TODO: encryption stuff

    socket_.connect(packet->ipaddr_, packet->port_);
    socket_.setUseDecompress(true);

    socket_.writeSeed(0);

    Config& config = Client::getSingleton()->getConfig();
    packets::GameServerLoginRequest loginReq(config["/uome/shard/account@name"].asString(),
            config["/uome/shard/account@password"].asString(),
            packet->encryptionKey_);
    socket_.write(loginReq);
}

uint32_t Manager::getSeed() {
    return socket_.getSeed();
}

}
}
