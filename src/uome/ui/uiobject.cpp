
#include "uiobject.hpp"

namespace uome {
namespace ui {

UiObject::UiObject(int locX, int locY) {
    setLocation(locX, locY);
}

int UiObject::getLocX() {
    return locX_;
}

int UiObject::getLocY() {
    return locY_;
}

void UiObject::setLocation(int locX, int locY) {
    locX_ = locX;
    locY_ = locY;
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
