#ifndef FLUO_UI_PROPERTYLABEL_HPP
#define FLUO_UI_PROPERTYLABEL_HPP

#include "label.hpp"

namespace fluo {

namespace world {
    class Mobile;
}

namespace ui {
namespace components {

class PropertyLabel : public Label {
public:
    PropertyLabel(CL_GUIComponent* parent, const UnicodeString& linkName);

    void update(world::Mobile* mob);

private:
    UnicodeString linkName_;
};

}
}
}

#endif
