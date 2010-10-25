
#include "ingameobject.hpp"

namespace uome {
namespace world {

IngameObject::IngameObject() : visible_(false) {
}

void IngameObject::setLocation(int locX, int locY, int locZ) {
    CL_Vec3<int> oldLocation = location_;

    location_ = CL_Vec3<int>(locX, locY, locZ);

}

}
}
