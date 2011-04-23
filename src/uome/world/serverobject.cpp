
#include "serverobject.hpp"

namespace uome {
namespace world {


ServerObject::ServerObject(Serial serial) : serial_(serial) {
}

Serial ServerObject::getSerial() {
    return serial_;
}

}
}
