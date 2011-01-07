
#include "uiobject.hpp"

namespace uome {
namespace ui {

UiObject::UiObject(CL_GUIManager* manager) : CL_GUIComponent(manager, CL_GUITopLevelDescription()) {
}

UiObject::UiObject(CL_GUIComponent* parent) : CL_GUIComponent(parent) {
}

bool UiObject::isMovable() {
    return movable_;
}

void UiObject::setMovable(bool movable) {
    movable_ = movable;
}

bool UiObject::isResizable() {
    return resizable_;
}

void UiObject::setResizable(bool resizable) {
    resizable_ = resizable;
}

bool UiObject::isClosable() {
    return closable_;
}

void UiObject::setClosable(bool closable) {
    closable_ = closable;
}

}
}
