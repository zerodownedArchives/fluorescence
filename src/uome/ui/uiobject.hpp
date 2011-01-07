#ifndef UOME_UI_UIOBJECT_HPP
#define UOME_UI_UIOBJECT_HPP


namespace uome {
namespace ui {

class UiObject {
public:
    UiObject(int locX = 100, int locY = 100);

    int getLocX();
    int getLocY();

    void setLocation(int locX, int locY);

    bool isMovable();
    void setMovable(bool movable);

    bool isResizable();
    void setResizable(bool resizable);

    bool isClosable();
    void setClosable(bool closable);

private:
    int locX_;
    int locY_;

    bool movable_;
    bool resizable_;
    bool closable_;
};

}
}

#endif
