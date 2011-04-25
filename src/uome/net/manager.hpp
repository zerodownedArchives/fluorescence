#ifndef UOME_NET_MANAGER_HPP
#define UOME_NET_MANAGER_HPP

#include <boost/program_options/variables_map.hpp>
#include <boost/shared_ptr.hpp>

#include "packet.hpp"

namespace uome {
namespace net {

class Manager {
public:
    static bool create(const boost::program_options::variables_map& config);
    static Manager* getSingleton();

    static boost::shared_ptr<Packet> createPacket(uint8_t id);

private:
    static Manager* singleton_;
    Manager(const boost::program_options::variables_map& config);
    Manager(const Manager& copy) { }
    Manager& operator=(const Manager& copy) { return *this; }

};

}
}

#endif
