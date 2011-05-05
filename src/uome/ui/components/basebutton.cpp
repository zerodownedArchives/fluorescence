
#include "basebutton.hpp"

namespace uome {
namespace ui {
namespace components {

BaseButton::BaseButton(CL_GUIComponent* parent) : CL_PushButton(parent) {
    func_clicked().set(this, &BaseButton::onClicked, this);
}

UnicodeString BaseButton::getText() {
    return StringConverter::fromUtf8(get_text());
}

void BaseButton::setText(const UnicodeString& text) {
    set_text(StringConverter::toUtf8String(text));
}

}
}
}
