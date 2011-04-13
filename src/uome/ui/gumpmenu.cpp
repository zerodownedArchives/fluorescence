
#include "gumpmenu.hpp"

#include "manager.hpp"

#include <misc/logger.hpp>

namespace uome {
namespace ui {

GumpMenu::GumpMenu(const CL_GUITopLevelDescription& desc) :
    CL_Window(ui::Manager::getSingleton()->getGuiManager().get(), desc),
    activePageId_(0), firstPageId_(0) {

    addPage(0);
}

void GumpMenu::addPage(unsigned int pageId) {
    std::map<unsigned int, std::vector<CL_GUIComponent*> >::iterator it = pages_.find(pageId);

    if (it == pages_.end()) {
        // page does not exist yet
        pages_[pageId] = std::vector<CL_GUIComponent*>();
    }

    activatePage(pageId);

    if (firstPageId_ == 0) {
        firstPageId_ = pageId;
    }
}

void GumpMenu::addToCurrentPage(CL_GUIComponent* component) {
    pages_[activePageId_].push_back(component);
}

void GumpMenu::activateFirstPage() {
    if (activePageId_ == firstPageId_) {
        return;
    }

    std::map<unsigned int, std::vector<CL_GUIComponent*> >::iterator iter = pages_.begin();
    std::map<unsigned int, std::vector<CL_GUIComponent*> >::iterator end = pages_.end();

    for (; iter != end; ++iter) {
        if (iter->first != 0 && iter->first != firstPageId_) {
            internalDeactivatePage(iter->first);
        }
    }

    activatePage(firstPageId_);
}

void GumpMenu::activatePage(unsigned int pageId) {
    if (pageId == activePageId_) {
        return;
    }

    if (activePageId_ != 0) {
        internalDeactivatePage(activePageId_);
    }

    activePageId_ = pageId;
    internalActivatePage(activePageId_);
}

unsigned int GumpMenu::getActivePageId() {
    return activePageId_;
}

void GumpMenu::onButtonClicked(components::BaseButton* button) {
    if (button->getPageId()) {
        activatePage(button->getPageId());
    } else {
        LOGARG_INFO(LOGTYPE_UI, "Button clicked, id=%i", button->getButtonId());
    }
}

void GumpMenu::internalActivatePage(unsigned int pageId) {
    std::vector<CL_GUIComponent*>::iterator iter = pages_[pageId].begin();
    std::vector<CL_GUIComponent*>::iterator end = pages_[pageId].end();

    for (; iter != end; ++iter) {
        (*iter)->set_visible(true);
    }
}

void GumpMenu::internalDeactivatePage(unsigned int pageId) {
    std::vector<CL_GUIComponent*>::iterator iter = pages_[pageId].begin();
    std::vector<CL_GUIComponent*>::iterator end = pages_[pageId].end();

    for (; iter != end; ++iter) {
        (*iter)->set_visible(false);
    }
}


}
}
