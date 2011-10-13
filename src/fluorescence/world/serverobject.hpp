#ifndef FLUO_WORLD_SERVEROBJECT_HPP
#define FLUO_WORLD_SERVEROBJECT_HPP

#include "ingameobject.hpp"

#include <typedefs.hpp>

namespace fluo {
namespace world {

class ServerObject : public IngameObject {
public:
    ServerObject(Serial serial, unsigned int objectType);

    Serial getSerial() const;

private:
    Serial serial_;
};

}
}

#endif
