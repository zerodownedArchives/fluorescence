
#include "gumpmenu.hpp"

#include "manager.hpp"
#include "gumpactions.hpp"
#include "cursormanager.hpp"
#include "components/propertylabel.hpp"

#include <client.hpp>

#include <misc/log.hpp>

#include <world/mobile.hpp>

namespace fluo {
namespace ui {

GumpMenu::GumpMenu(const CL_GUITopLevelDescription& desc) :
    CL_Window(ui::Manager::getSingleton()->getGuiManager().get(), desc),
    activePageId_(0), firstPageId_(0),
    closable_(true),
    draggable_(true), isDragged_(false),
    linkedMobile_(NULL) {

    addPage(0);

    func_input_pressed().set(this, &GumpMenu::onInputPressed);
    func_input_released().set(this, &GumpMenu::onInputReleased);
    func_input_pointer_moved().set(this, &GumpMenu::onPointerMoved);

    ui::Manager::getSingleton()->registerGumpMenu(this);
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

void GumpMenu::removeFromPages(const CL_GUIComponent* component) {
    std::map<unsigned int, std::vector<CL_GUIComponent*> >::iterator iter = pages_.begin();
    std::map<unsigned int, std::vector<CL_GUIComponent*> >::iterator end = pages_.end();

    for (; iter != end; ++iter) {
        std::remove(iter->second.begin(), iter->second.end(), component);
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

bool GumpMenu::onInputPressed(const CL_InputEvent& msg) {
    //LOGARG_INFO(LOGTYPE_INPUT, "input pressed gumpmenu: %u", msg.id);
    bool consumed = true;

    switch (msg.id) {
    case CL_MOUSE_LEFT:
        if (!draggable_) {
            consumed = false;
            break;
        }

        startDragging(msg.mouse_pos);

        break;

#ifndef WIN32
    case CL_KEY_NUMPAD_ENTER:
#endif
    case CL_KEY_ENTER:
        if (action_.length() > 0) {
            GumpActions::doAction(this, action_, 0, NULL);
        } else {
            consumed = false;
        }
        break;

    case CL_KEY_ESCAPE:
        if (cancelAction_.length() > 0) {
            GumpActions::doAction(this, cancelAction_, 0, NULL);
        } else {
            consumed = false;
        }
        break;

    default:
        consumed = false;
        break;
    }

    return false;
}

bool GumpMenu::onInputReleased(const CL_InputEvent& msg) {
    if (msg.id == CL_MOUSE_LEFT) {
        if (!draggable_) {
            return false;
        }

        isDragged_ = false;
        capture_mouse(false);

        return true;
    } else if (msg.id == CL_MOUSE_RIGHT && closable_) {
        capture_mouse(false);
        ui::Manager::getSingleton()->closeGumpMenu(this);

        return true;
    }

    return false;
}

bool GumpMenu::onPointerMoved(const CL_InputEvent& msg) {
    if (isDragged_) {
        CL_Rect geometry = get_window_geometry();
        geometry.translate(msg.mouse_pos.x - lastMousePos_.x, msg.mouse_pos.y - lastMousePos_.y);
        set_window_geometry(geometry);

        return true;
    } else if (Client::getSingleton()->getState() != Client::STATE_SHARD_SELECTION) {
        ui::Manager::getSingleton()->getCursorManager()->onCursorMove(component_to_screen_coords(msg.mouse_pos));
        return true;
    }
    return false;
}

void GumpMenu::setClosable(bool value) {
    closable_ = value;
}

bool GumpMenu::isClosable() {
    return closable_;
}

void GumpMenu::setDraggable(bool value) {
    draggable_ = value;

    if (!draggable_) {
        capture_mouse(false);
        isDragged_ = false;
    }
}

bool GumpMenu::isDraggable() {
    return draggable_;
}

void GumpMenu::setName(const UnicodeString& name) {
    name_ = name;
}

const UnicodeString& GumpMenu::getName() {
    return name_;
}

void GumpMenu::setAction(const UnicodeString& action) {
    action_ = action;
}

void GumpMenu::setCancelAction(const UnicodeString& action) {
    cancelAction_ = action;
}

void GumpMenu::updateMobileProperties() {
    if (!linkedMobile_) {
        LOG_WARN << "Calling updateMobileProperties on gump without linked mobile " << name_ << std::endl;
        ui::Manager::getSingleton()->closeGumpMenu(this);
        return;
    }

    std::vector<CL_GUIComponent*> children = get_child_components();

    std::vector<CL_GUIComponent*>::iterator iter = children.begin();
    std::vector<CL_GUIComponent*>::iterator end = children.end();

    components::PropertyLabel* lbl;
    for (; iter != end; ++iter) {
        lbl = dynamic_cast<components::PropertyLabel*>(*iter);
        if (lbl) {
            lbl->update(linkedMobile_);
        }
    }
}

void GumpMenu::setLinkedMobile(world::Mobile* mob) {
    linkedMobile_ = mob;
    if (linkedMobile_) {
        updateMobileProperties();
    }
}

void GumpMenu::onClose() {
    if (linkedMobile_) {
        linkedMobile_->removeLinkedGump(this);
    }
}

void GumpMenu::startDragging(const CL_Point& mousePos) {
    bring_to_front();
    capture_mouse(true);
    lastMousePos_ = mousePos;
    isDragged_ = true;
}

}
}
