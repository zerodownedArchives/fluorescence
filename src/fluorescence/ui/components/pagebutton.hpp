#ifndef FLUO_UI_COMPONENTS_PAGEBUTTON_HPP
#define FLUO_UI_COMPONENTS_PAGEBUTTON_HPP

#include "basebutton.hpp"

namespace fluo {
namespace ui {
namespace components {

class PageButton : public BaseButton {
public:
    PageButton(CL_GUIComponent* parent, unsigned int pageId);

    unsigned int getPageId();

    virtual void onClicked(BaseButton* self);

private:
    unsigned int pageId_;
};

}
}
}

#endif
