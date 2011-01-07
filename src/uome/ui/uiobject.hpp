#ifndef UOME_UI_UIOBJECT_HPP
#define UOME_UI_UIOBJECT_HPP

#include <list>

#include <ClanLib/GUI/gui_component.h>
#include <ClanLib/GUI/gui_manager.h>

namespace uome {
namespace ui {

class UiObject : public CL_GUIComponent {
public:
    UiObject(CL_GUIManager* manager);
    UiObject(CL_GUIComponent* parent);

    bool isMovable();
    void setMovable(bool movable);

    bool isResizable();
    void setResizable(bool resizable);

    bool isClosable();
    void setClosable(bool closable);

private:
    bool movable_;
    bool resizable_;
    bool closable_;
};

}
}

#endif
