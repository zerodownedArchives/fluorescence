#ifndef UOME_WORLD_SERVEROBJECT_HPP
#define UOME_WORLD_SERVEROBJECT_HPP

#include "ingameobject.hpp"

namespace uome {
namespace world {

typedef unsigned int Serial;

class ServerObject : public IngameObject {
public:
    ServerObject(Serial serial);

    Serial getSerial();

private:
    Serial serial_;
};

}
}

#endif
