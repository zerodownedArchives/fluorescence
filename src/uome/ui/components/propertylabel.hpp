#ifndef UOME_UI_PROPERTYLABEL_HPP
#define UOME_UI_PROPERTYLABEL_HPP

#include "label.hpp"

namespace uome {

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
