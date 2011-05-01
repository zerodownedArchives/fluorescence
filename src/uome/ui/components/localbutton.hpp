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

    void setAction(const std::string& value);
    const std::string& getAction();

    void setParameter(const std::string& value);
    const std::string& getParameter();

    virtual void onClicked(BaseButton* self);

private:
    std::string action_;
    std::string parameter_;

    struct ClickAction {
        ClickAction();

        ClickAction(bool closeGumpMenu, boost::function<bool (GumpMenu*, const std::string&)> callback);

        bool closeGumpMenu_;
        boost::function<bool (GumpMenu*, const std::string&)> callback_;
    };

    static std::map<std::string, ClickAction> actionTable_;

    static bool closeHelper(GumpMenu* menu, const std::string& param);
};

}
}
}

#endif
