#ifndef UOME_UI_COMPONENTS_LOCALBUTTON_HPP
#define UOME_UI_COMPONENTS_LOCALBUTTON_HPP

#include "basebutton.hpp"

#include <ui/gumpmenu.hpp>

#include <boost/function.hpp>
#include <map>

namespace uome {
namespace ui {
namespace components {

class LocalButton : public BaseButton {
public:
    static void buildBasicActionTable();
    static void buildFullActionTable();

    LocalButton(CL_GUIComponent* parent, const std::string& action, const std::string& parameter = "");

    const std::string& getAction();
    const std::string& getParameter();

    virtual void onClicked(BaseButton* self);

private:
    std::string action_;
    std::string parameter_;

    struct ClickAction {
        ClickAction();

        ClickAction(bool closeGumpMenu, boost::function<void (GumpMenu*, const std::string&)> callback);

        bool closeGumpMenu_;
        boost::function<void (GumpMenu*, const std::string&)> callback_;
    };

    static std::map<std::string, ClickAction> actionTable_;
};

}
}
}

#endif
