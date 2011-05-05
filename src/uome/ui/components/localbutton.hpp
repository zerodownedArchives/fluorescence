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

    LocalButton(CL_GUIComponent* parent, const UnicodeString& action, const UnicodeString& parameter);

    void setAction(const UnicodeString& value);
    const UnicodeString& getAction() const;

    void setParameter(const UnicodeString& value);
    const UnicodeString& getParameter() const;
    int getParameterInt() const;

    virtual void onClicked(BaseButton* self);

private:
    UnicodeString action_;
    UnicodeString parameter_;

    struct ClickAction {
        ClickAction();

        ClickAction(bool closeGumpMenu, boost::function<bool (GumpMenu*, LocalButton*)> callback);

        bool closeGumpMenu_;
        boost::function<bool (GumpMenu*, LocalButton*)> callback_;
    };

    static std::map<UnicodeString, ClickAction> actionTable_;

    static bool closeHelper(GumpMenu* menu, LocalButton* button);
};

}
}
}

#endif
