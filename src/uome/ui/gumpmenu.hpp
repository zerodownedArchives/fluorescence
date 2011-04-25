#ifndef UOME_UI_GUMPMENU_HPP
#define UOME_UI_GUMPMENU_HPP

#include <ClanLib/gui.h>

#include <boost/enable_shared_from_this.hpp>

#include <map>

#include "components/basebutton.hpp"
#include "buttonclickhandler.hpp"

namespace uome {
namespace ui {

class GumpMenu : public CL_Window, public boost::enable_shared_from_this<GumpMenu> {
public:
    GumpMenu(const CL_GUITopLevelDescription& desc);

    void addPage(unsigned int pageId);

    void addToCurrentPage(CL_GUIComponent* component);

    void activatePage(unsigned int pageId);
    void activateFirstPage();

    unsigned int getActivePageId();

    void onButtonClicked(components::BaseButton* button);

    void setClosable(bool value);
    bool isClosable();

    void setDraggable(bool value);
    bool isDraggable();

    void setButtonClickHandler(boost::shared_ptr<ButtonClickHandler> handler);

private:
    unsigned int activePageId_;
    std::map<unsigned int, std::vector<CL_GUIComponent*> > pages_;
    unsigned int firstPageId_;

    void internalActivatePage(unsigned int pageId);
    void internalDeactivatePage(unsigned int pageId);

    bool closable_;

    bool draggable_;
    bool isDragged_;
    CL_Point lastMousePos_;
    bool onInputPressed(const CL_InputEvent& msg);
    bool onInputReleased(const CL_InputEvent& msg);
    bool onPointerMoved(const CL_InputEvent& msg);

    boost::shared_ptr<ButtonClickHandler> buttonClickHandler_;
};

}
}

#endif
