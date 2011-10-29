
#include "target.hpp"

#include "world/dynamicitem.hpp"

namespace fluo {
namespace ui {
namespace targeting {

Target::Target(bool allowGround) : allowGround_(allowGround) {
}

bool Target::acceptsTarget(boost::shared_ptr<world::IngameObject> obj) const {
    if (allowGround_) {
        return obj->isDynamicItem() || obj->isMobile() || obj->isStaticItem() || obj->isMap();
    } else {
        return obj->isDynamicItem() || obj->isMobile();
    }
}

}
}
}
