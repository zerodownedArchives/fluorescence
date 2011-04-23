#ifndef UOME_NET_MANAGER_HPP
#define UOME_NET_MANAGER_HPP


namespace uome {
namespace net {

class Manager {
public:
    static Manager* getSingleton();
private:
    static Manager* singleton_;
    Manager();
    Manager(const Manager& copy) { }
    Manager& operator=(const Manager& copy) { return *this; }

};

}
}

#endif
