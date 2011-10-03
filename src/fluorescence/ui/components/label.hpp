#ifndef FLUO_UI_COMPONENTS_LABEL_HPP
#define FLUO_UI_COMPONENTS_LABEL_HPP

#include <ClanLib/GUI/Components/label.h>

#include <misc/string.hpp>

namespace fluo {
namespace ui {
namespace components {

class Label : public CL_Label {
public:
    Label(CL_GUIComponent* parent);

    void setText(const UnicodeString& string);
    UnicodeString getText();
};

}
}
}

#endif
