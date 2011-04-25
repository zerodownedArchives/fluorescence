#ifndef UOME_UI_COMPONENTS_PAGEBUTTON_HPP
#define UOME_UI_COMPONENTS_PAGEBUTTON_HPP

#include "basebutton.hpp"

namespace uome {
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
