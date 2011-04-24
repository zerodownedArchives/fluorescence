#ifndef UOME_NET_MANAGER_HPP
#define UOME_NET_MANAGER_HPP

#include <boost/program_options/variables_map.hpp>

namespace uome {
namespace net {

class Manager {
public:
    static bool create(const boost::program_options::variables_map& config);
    static Manager* getSingleton();
private:
    static Manager* singleton_;
    Manager(const boost::program_options::variables_map& config);
    Manager(const Manager& copy) { }
    Manager& operator=(const Manager& copy) { return *this; }

};

}
}

#endif
