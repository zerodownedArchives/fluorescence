
#include "label.hpp"

namespace uome {
namespace ui {
namespace components {

Label::Label(CL_GUIComponent* parent) : CL_Label(parent) {
}

UnicodeString Label::getText() {
    return StringConverter::fromUtf8(get_text());
}

void Label::setText(const UnicodeString& text) {
    set_text(StringConverter::toUtf8String(text));
}

}
}
}
