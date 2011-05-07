#ifndef UOME_UI_COMPONENTS_LABEL_HPP
#define UOME_UI_COMPONENTS_LABEL_HPP

#include <ClanLib/GUI/Components/label.h>

#include <misc/string.hpp>

namespace uome {
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
