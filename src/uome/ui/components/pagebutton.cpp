
#include "pagebutton.hpp"

#include <misc/log.hpp>

#include <ui/gumpmenu.hpp>

namespace uome {
namespace ui {
namespace components {

PageButton::PageButton(CL_GUIComponent* parent, unsigned int pageId) : BaseButton(parent), pageId_(pageId) {
}

unsigned int PageButton::getPageId() {
    return pageId_;
}

void PageButton::onClicked(BaseButton* self) {
    GumpMenu* gump = dynamic_cast<GumpMenu*>(get_top_level_component());
    if (gump) {
        gump->activatePage(getPageId());
    } else {
        LOG_ERROR << "PageButton inside something other than GumpMenu" << std::endl;
    }
}

}
}
}
