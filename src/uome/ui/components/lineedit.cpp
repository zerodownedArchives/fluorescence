
#include "lineedit.hpp"

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

}
}
}
