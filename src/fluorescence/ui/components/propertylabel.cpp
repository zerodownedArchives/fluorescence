
#include "propertylabel.hpp"

#include <world/mobile.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace components {

PropertyLabel::PropertyLabel(CL_GUIComponent* parent, const UnicodeString& link) : Label(parent), linkName_(link) {
}

void PropertyLabel::update(world::Mobile* mob) {
    if (mob->hasProperty(linkName_)) {
        setText(mob->getProperty(linkName_).asString());
    }
}

}
}
}
