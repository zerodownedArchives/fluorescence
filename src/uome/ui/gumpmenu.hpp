#ifndef UOME_UI_GUMPMENU_HPP
#define UOME_UI_GUMPMENU_HPP

#include <ClanLib/gui.h>
#include <boost/enable_shared_from_this.hpp>
#include <map>

#include <misc/config.hpp>

#include "components/basebutton.hpp"
#include "gumpmenus.hpp"

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

    void setClosable(bool value);
    bool isClosable();

    void setDraggable(bool value);
    bool isDraggable();

    void setName(const std::string& name);
    const std::string& getName();


    template<class ComponentClass>
    void setComponentTextFromConfig(const std::string& componentName, const std::string& configName, Config* config) {
        if (config->count(configName) > 0) {
            setComponentText<ComponentClass>(componentName, config->get(configName).as<std::string>());
        }
    }

    template<class ComponentClass>
    void setComponentText(const std::string& componentName, const std::string& text) {
        ComponentClass* component = dynamic_cast<ComponentClass*>(get_named_item(componentName));
        if (!component) {
            std::string errorMessage = "Unable to find field \"" + componentName + "\" in gump " + getName();
            GumpMenus::openMessageBox(errorMessage);
        } else {
            component->set_text(text);
        }
    }



    template<class ComponentClass>
    bool getComponentText(const std::string& componentName, std::string& result) {
        ComponentClass* component = dynamic_cast<ComponentClass*>(get_named_item(componentName));
        if (!component) {
            std::string errorMessage = "Unable to find field \"" + componentName + "\" in gump " + getName();
            GumpMenus::openMessageBox(errorMessage);
            return false;
        } else {
            result = component->get_text();
            return true;
        }
    }

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

    std::string name_;
};

}
}

#endif
