
#include "ingameobject.hpp"

namespace uome {
namespace world {

IngameObject::IngameObject() : visible_(false) {
}

IngameObject::IngameObject(int locX, int locY, int locZ) :
        location_(locX, locY, locZ), visible_(true) {
}

void IngameObject::setLocation(int locX, int locY, int locZ) {
    CL_Vec3<int> oldLocation = location_;

    location_ = CL_Vec3(locX, locY, locZ);

}
}
