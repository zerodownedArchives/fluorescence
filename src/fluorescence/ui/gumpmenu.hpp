#ifndef FLUO_UI_GUMPMENU_HPP
#define FLUO_UI_GUMPMENU_HPP

#include <ClanLib/gui.h>
#include <boost/enable_shared_from_this.hpp>
#include <map>

#include <misc/config.hpp>

#include "components/basebutton.hpp"
#include "gumpmenus.hpp"

namespace fluo {
namespace world {
    class Mobile;
}

namespace ui {

class GumpMenu : public CL_Window, public boost::enable_shared_from_this<GumpMenu> {
public:
    GumpMenu(const CL_GUITopLevelDescription& desc);

    void addPage(unsigned int pageId);

    void removeFromPages(const CL_GUIComponent* component);
    void addToCurrentPage(CL_GUIComponent* component);

    void activatePage(unsigned int pageId);
    void activateFirstPage();

    unsigned int getActivePageId();

    void setClosable(bool value);
    bool isClosable();

    void setDraggable(bool value);
    bool isDraggable();

    void setName(const UnicodeString& name);
    const UnicodeString& getName();

    void setAction(const UnicodeString& action);
    void setCancelAction(const UnicodeString& action);


    template<class ComponentClass>
    void setComponentTextFromConfig(const UnicodeString& componentName, const UnicodeString& configName, Config& config) {
        if (config.exists(configName) > 0) {
            setComponentText<ComponentClass>(componentName, config[configName].asString());
        }
    }

    template<class ComponentClass>
    void setComponentText(const UnicodeString& componentName, const UnicodeString& text) {
        std::string utf8CompName = StringConverter::toUtf8String(componentName);
        ComponentClass* component = dynamic_cast<ComponentClass*>(get_named_item(utf8CompName));
        if (!component) {
            UnicodeString errorMessage = UnicodeString("Unable to find field \"").append(componentName).append("\" in gump ").append(getName());
            GumpMenus::openMessageBox(errorMessage);
        } else {
            component->setText(text);
        }
    }

    template<class ComponentClass>
    bool getComponentText(const UnicodeString& componentName, UnicodeString& result) {
        std::string utf8CompName = StringConverter::toUtf8String(componentName);
        ComponentClass* component = dynamic_cast<ComponentClass*>(get_named_item(utf8CompName));
        if (!component) {
            UnicodeString errorMessage = UnicodeString("Unable to find field \"").append(componentName).append("\" in gump ").append(getName());
            GumpMenus::openMessageBox(errorMessage);
            return false;
        } else {
            result = component->getText();
            return true;
        }
    }

    void updateMobileProperties();
    void setLinkedMobile(world::Mobile* mob);

    void onClose();

    void startDragging(const CL_Point& mousePos);

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

    UnicodeString name_;

    UnicodeString action_;
    UnicodeString cancelAction_;

    world::Mobile* linkedMobile_;
};

}
}

#endif
