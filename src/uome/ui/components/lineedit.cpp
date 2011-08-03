
#include "lineedit.hpp"

#include <ui/gumpmenu.hpp>
#include <ui/gumpactions.hpp>

namespace uome {
namespace ui {
namespace components {

LineEdit::LineEdit(CL_GUIComponent* parent) : CL_LineEdit(parent) {
}

UnicodeString LineEdit::getText() {
    return StringConverter::fromUtf8(get_text());
}

void LineEdit::setText(const UnicodeString& text) {
    set_text(StringConverter::toUtf8String(text));
}

void LineEdit::setAction(const UnicodeString& action) {
    action_ = action;
    func_enter_pressed().set(this, &LineEdit::onEnterPressed);
}

void LineEdit::onEnterPressed() {
    if (action_.length() > 0) {
        GumpMenu* gump = dynamic_cast<GumpMenu*>(get_top_level_component());
        if (gump) {
            GumpActions::doAction(gump, action_, 0, NULL);
        }
    }
}

}
}
}
