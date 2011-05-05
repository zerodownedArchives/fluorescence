#ifndef UOME_UI_COMPONENTS_LINEEDIT_HPP
#define UOME_UI_COMPONENTS_LINEEDIT_HPP

#include <ClanLib/GUI/Components/lineedit.h>

#include <misc/string.hpp>

namespace uome {
namespace ui {
namespace components {

class LineEdit : public CL_LineEdit {
public:
    LineEdit(CL_GUIComponent* parent);

    void setText(const UnicodeString& string);
    UnicodeString getText();
};

}
}
}

#endif
