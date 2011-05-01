#ifndef UOME_UI_SCROLLAREA_HPP
#define UOME_UI_SCROLLAREA_HPP

#include <ClanLib/gui.h>

namespace uome {
namespace ui {
namespace components {

class ScrollArea : public CL_GUIComponent {
public:
    static const unsigned int VISIBLE_ON_DEMAND = 0;
    static const unsigned int VISIBLE_ALWAYS = 1;
    static const unsigned int VISIBLE_NEVER = 2;

    ScrollArea(CL_GUIComponent* parent);

    CL_Frame* getClientArea();
    CL_ScrollBar* getVerticalScrollBar();
    CL_ScrollBar* getHorizontalScrollBar();

    /**
     * \brief Calculate scroll bar visibility and parameters
     * \param verticalPageStep 0 for auto, value to control vertical page step size directly
     * \param horizontalPageStep 0 for auto, value to control horizontal page step size directly
     */
    void updateScrollbars(unsigned int verticalVisibility = VISIBLE_ON_DEMAND, unsigned int horizontalVisibility = VISIBLE_ON_DEMAND,
            unsigned int verticalPageStep = 0, unsigned int horizontalPageStep = 0,
            unsigned int verticalLineStep = 0, unsigned int horizontalLineStep = 0,
            unsigned int marginLeft = 0, unsigned int marginBottom = 0);

private:
    CL_ScrollBar* verticalScrollBar_;
    CL_ScrollBar* horizontalScrollBar_;

    CL_Frame* clientArea_;

    void onScroll();
};

}
}
}

#endif
