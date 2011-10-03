#ifndef FLUO_UI_COMPONENTS_LINEEDIT_HPP
#define FLUO_UI_COMPONENTS_LINEEDIT_HPP

#include <ClanLib/GUI/Components/lineedit.h>

#include <misc/string.hpp>

namespace fluo {
namespace ui {
namespace components {

class LineEdit : public CL_LineEdit {
public:
    LineEdit(CL_GUIComponent* parent);

    void setText(const UnicodeString& string);
    UnicodeString getText();

    void setAction(const UnicodeString& action);
    void onEnterPressed();

private:
    UnicodeString action_;

};

}
}
}

#endif
