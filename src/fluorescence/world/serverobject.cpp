
#include "serverobject.hpp"

#include <data/manager.hpp>
#include <data/huesloader.hpp>

namespace fluo {
namespace world {


ServerObject::ServerObject(Serial serial, unsigned int objectType) : IngameObject(objectType), serial_(serial), hue_(0) {
    draggable_ = true;
}

Serial ServerObject::getSerial() const {
    return serial_;
}

void ServerObject::setHue(unsigned int hue) {
    if (hue != hue_) {
        hue_ = hue;
        worldRenderData_.hueInfo_[1u] = data::Manager::getHuesLoader()->translateHue(hue_);
    }
}

}
}
