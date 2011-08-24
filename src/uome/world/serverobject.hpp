#ifndef UOME_WORLD_SERVEROBJECT_HPP
#define UOME_WORLD_SERVEROBJECT_HPP

#include "ingameobject.hpp"

#include <typedefs.hpp>

namespace uome {
namespace world {

class ServerObject : public IngameObject {
public:
    ServerObject(Serial serial);

    Serial getSerial() const;

private:
    Serial serial_;
};

}
}

#endif
