#ifndef FLUO_UI_COMPONENTS_LOCALBUTTON_HPP
#define FLUO_UI_COMPONENTS_LOCALBUTTON_HPP

#include "basebutton.hpp"

#include <ui/gumpmenu.hpp>

namespace fluo {
namespace ui {
namespace components {

class LocalButton : public BaseButton {
public:
    static const unsigned int MAX_PARAMETER_COUNT = 5;

    LocalButton(CL_GUIComponent* parent, const UnicodeString& action);

    void setAction(const UnicodeString& value);
    const UnicodeString& getAction() const;

    void setParameter(const UnicodeString& value, unsigned int index = 0);
    const UnicodeString& getParameter(unsigned int index = 0) const;
    int getParameterInt(unsigned int index = 0) const;
    const UnicodeString* getParameterPtr() const;

    virtual void onClicked(BaseButton* self);

private:
    UnicodeString action_;
    UnicodeString parameter_[MAX_PARAMETER_COUNT];
};

}
}
}

#endif
