
#include "serverobject.hpp"

namespace fluo {
namespace world {


ServerObject::ServerObject(Serial serial, unsigned int objectType) : IngameObject(objectType), serial_(serial) {
    draggable_ = true;
}

Serial ServerObject::getSerial() const {
    return serial_;
}

}
}
