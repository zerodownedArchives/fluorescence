/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "gumpmenu.hpp"

#include "manager.hpp"
#include "gumpactions.hpp"
#include "cursormanager.hpp"
#include "python/scriptloader.hpp"
#include "components/lineedit.hpp"
#include "components/propertylabel.hpp"
#include "components/checkbox.hpp"

#include <client.hpp>
#include <misc/log.hpp>
#include <world/mobile.hpp>
#include <net/manager.hpp>
#include <net/packets/b1_gumpreply.hpp>

namespace fluo {
namespace ui {

GumpMenu::GumpMenu(const CL_GUITopLevelDescription& desc) :
    CL_Window(ui::Manager::getSingleton()->getGuiManager().get(), desc),
    serial_(0), typeId_(0), activePageId_(0), firstPageId_(0),
    closable_(true),
    draggable_(true), isDragged_(false),
    currentRadioGroup_(0) {

    addPage(0);

    func_input_pressed().set(this, &GumpMenu::onInputPressed);
    func_input_released().set(this, &GumpMenu::onInputReleased);
    func_input_pointer_moved().set(this, &GumpMenu::onPointerMoved);

    ui::Manager::getSingleton()->registerGumpMenu(this);

    setFontB("unifont1", 12, false);
}

GumpMenu::~GumpMenu() {
}

void GumpMenu::addPage(unsigned int pageId) {
    std::map<unsigned int, std::vector<GumpComponent*> >::iterator it = pages_.find(pageId);

    if (it == pages_.end()) {
        // page does not exist yet
        pages_[pageId] = std::vector<GumpComponent*>();
    }

    activatePage(pageId);

    if (firstPageId_ == 0) {
        firstPageId_ = pageId;
    }
}

void GumpMenu::removeFromPages(const GumpComponent* component) {
    std::map<unsigned int, std::vector<GumpComponent*> >::iterator iter = pages_.begin();
    std::map<unsigned int, std::vector<GumpComponent*> >::iterator end = pages_.end();

    for (; iter != end; ++iter) {
        std::remove(iter->second.begin(), iter->second.end(), component);
    }
}

void GumpMenu::addToCurrentPage(GumpComponent* component) {
    pages_[activePageId_].push_back(component);
    component->func_resized().set(this, &GumpMenu::fitSizeToChildren);
}

void GumpMenu::activateFirstPage() {
    if (activePageId_ == firstPageId_) {
        return;
    }

    std::map<unsigned int, std::vector<GumpComponent*> >::iterator iter = pages_.begin();
    std::map<unsigned int, std::vector<GumpComponent*> >::iterator end = pages_.end();

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

    setCurrentRadioGroup(pageId);
}

unsigned int GumpMenu::getActivePageId() {
    return activePageId_;
}

void GumpMenu::internalActivatePage(unsigned int pageId) {
    std::vector<GumpComponent*>::iterator iter = pages_[pageId].begin();
    std::vector<GumpComponent*>::iterator end = pages_[pageId].end();

    for (; iter != end; ++iter) {
        (*iter)->set_visible(true);
    }

    request_repaint();
}

void GumpMenu::internalDeactivatePage(unsigned int pageId) {
    std::vector<GumpComponent*>::iterator iter = pages_[pageId].begin();
    std::vector<GumpComponent*>::iterator end = pages_[pageId].end();

    for (; iter != end; ++iter) {
        (*iter)->set_visible(false);
    }
}

bool GumpMenu::onInputPressed(const CL_InputEvent& msg) {
    //LOG_INFO << "input pressed gumpmenu: " << msg.id << std::endl;
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
        if (pyEnterCallback_) {
            executePythonCallback(pyEnterCallback_);
        } else {
            consumed = false;
        }
        break;

    case CL_KEY_ESCAPE:
        if (pyEscapeCallback_) {
            executePythonCallback(pyEscapeCallback_);
        } else {
            consumed = false;
        }
        break;

    default:
        consumed = false;
        break;
    }

    if (!consumed) {
        consumed = ui::Manager::getSingleton()->onUnhandledInputEvent(msg);
    }

    return consumed;
}

bool GumpMenu::onInputReleased(const CL_InputEvent& msg) {
    bool consumed = false;
    if (msg.id == CL_MOUSE_LEFT && draggable_) {
        isDragged_ = false;
        capture_mouse(false);

        consumed = true;
    } else if (msg.id == CL_MOUSE_RIGHT && closable_) {
        capture_mouse(false);
        ui::Manager::getSingleton()->closeGumpMenu(this);

        consumed = true;
    }

    if (!consumed) {
        consumed = ui::Manager::getSingleton()->onUnhandledInputEvent(msg);
    }

    return consumed;
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

void GumpMenu::updateMobileProperties() {
    if (!linkedMobile_) {
        return;
    }

    updateMobilePropertiesRec(this);
}

void GumpMenu::updateMobilePropertiesRec(CL_GUIComponent* comp) {
    std::vector<CL_GUIComponent*> children = comp->get_child_components();

    std::vector<CL_GUIComponent*>::iterator iter = children.begin();
    std::vector<CL_GUIComponent*>::iterator end = children.end();

    for (; iter != end; ++iter) {
        components::PropertyLabel* lbl = dynamic_cast<components::PropertyLabel*>(*iter);
        if (lbl) {
            lbl->update(linkedMobile_);
        } else {
            // TODO: new gui
            //components::WarModeButton* wmbut = dynamic_cast<components::WarModeButton*>(*iter);
            //if (wmbut) {
                //wmbut->setWarMode(linkedMobile_->isWarMode());
            //}
        }

        updateMobilePropertiesRec(*iter);
    }
}

void GumpMenu::setLinkedMobile(const boost::shared_ptr<world::Mobile>& mob) {
    if (linkedMobile_ == mob) {
        return;
    }

    if (linkedMobile_) {
        linkedMobile_->removeLinkedGump(this);
    }

    linkedMobile_ = mob;

    if (linkedMobile_) {
        linkedMobile_->addLinkedGump(this);
        updateMobileProperties();
    }
}

boost::shared_ptr<world::Mobile> GumpMenu::getLinkedMobile() const {
    return linkedMobile_;
}

void GumpMenu::onClose() {
    if (linkedMobile_) {
        linkedMobile_->removeLinkedGump(this);
    }

    if (closeCallback_) {
        closeCallback_();
    }
}

void GumpMenu::startDragging(const CL_Point& mousePos) {
    capture_mouse(true);
    lastMousePos_ = mousePos;
    isDragged_ = true;
}

void GumpMenu::fitSizeToChildren() {
    int childWidth = 0;
    int childHeight = 0;

    std::vector<CL_GUIComponent*> children = get_child_components();
    std::vector<CL_GUIComponent*>::const_iterator iter = children.begin();
    std::vector<CL_GUIComponent*>::const_iterator end = children.end();
    for (; iter != end; ++iter) {
        CL_Pointx<int> cur = (*iter)->get_geometry().get_bottom_right();
        if (cur.x > childWidth) {
            childWidth = cur.x;
        }

        if (cur.y > childHeight) {
            childHeight = cur.y;
        }
    }

    childWidth = (std::max)(1, childWidth);
    childHeight = (std::max)(1, childHeight);

    CL_Rectf geom = get_geometry();
    if (geom.get_width() != childWidth || geom.get_height() != childHeight) {
        geom.set_width(childWidth);
        geom.set_height(childHeight);
        set_geometry(geom);
    }
}

void GumpMenu::setSerial(Serial serial) {
    if (serial_ == 0) {
        serial_ = serial;

        // TODO: register somewhere?
    }
}

Serial GumpMenu::getSerial() const {
    return serial_;
}

void GumpMenu::setTypeId(unsigned int typeId) {
    typeId_ = typeId;
}

unsigned int GumpMenu::getTypeId() const {
    return typeId_;
}

void GumpMenu::sendReply(unsigned int buttonId) {
    std::list<uint32_t> switches;
    std::list<net::packets::GumpReply::TextEntryInfo> textEntries;

    std::vector<CL_GUIComponent*> children = get_child_components();
    std::vector<CL_GUIComponent*>::const_iterator iter = children.begin();
    std::vector<CL_GUIComponent*>::const_iterator end = children.end();
    for (; iter != end; ++iter) {
        components::Checkbox* cb = dynamic_cast<components::Checkbox*>(*iter);
        if (cb && cb->isChecked()) {
            switches.push_back(cb->getSwitchId());
            continue;
        }

        components::LineEdit* entry = dynamic_cast<components::LineEdit*>(*iter);
        if (entry && entry->getEntryId() != 0xFFFFFFFFu) {
            net::packets::GumpReply::TextEntryInfo info;
            info.number_ = entry->getEntryId();
            info.text_ = entry->getText();
            textEntries.push_back(info);
        }
    }

    net::packets::GumpReply pkt(serial_, typeId_, buttonId, switches, textEntries);
    net::Manager::getSingleton()->send(pkt);

    ui::Manager::getSingleton()->closeGumpMenu(this);
}

void GumpMenu::setCurrentRadioGroup(unsigned int groupId) {
    currentRadioGroup_ = groupId;
}

unsigned int GumpMenu::getCurrentRadioGroup() const {
    return currentRadioGroup_;
}

bool GumpMenu::has_pixel(const CL_Point& p) const {
    return false;
}

void GumpMenu::setCloseCallback(boost::function<void()> cb) {
    closeCallback_ = cb;
}

void GumpMenu::setFont(const UnicodeString& name, unsigned int height) {
    setFontB(name, height, false);
}

void GumpMenu::setFontB(const UnicodeString& name, unsigned int height, bool border) {
    fontDesc_.set_typeface_name(StringConverter::toUtf8String(name));
    fontDesc_.set_height(height);
    fontDesc_.set_weight(border ? 700 : 400); // weight values taken from clanlib
    fontDesc_.set_subpixel(false);

    cachedFont_ = ui::Manager::getSingleton()->getFont(fontDesc_);
}

CL_Font GumpMenu::getFont() const {
    return cachedFont_;
}

GumpMenu* GumpMenu::create(const UnicodeString& name, int x, int y) {
    return createBackground(name, x, y, false);
}

GumpMenu* GumpMenu::createBackground(const UnicodeString& name, int x, int y, bool inBackground) {
    CL_GUITopLevelDescription desc(CL_Rect(x, y, CL_Size(1, 1)), false);
    desc.set_decorations(false);
    desc.set_in_background(inBackground);

    GumpMenu* gm = new GumpMenu(desc);
    gm->setName(name);

    return gm;
}

GumpComponent* GumpMenu::getNamedComponent(const UnicodeString& name) {
    CL_GUIComponent* cmp = get_named_item(StringConverter::toUtf8String(name));
    return dynamic_cast<GumpComponent*>(cmp);
}

boost::python::dict GumpMenu::getPythonStore() {
    return pythonStore_;
}

boost::python::object GumpMenu::getPyEnterCallback() const {
    return pyEnterCallback_;
}

void GumpMenu::setPyEnterCallback(boost::python::object cb) {
    pyEnterCallback_ = cb;
}

boost::python::object GumpMenu::getPyEscapeCallback() const {
    return pyEscapeCallback_;
}

void GumpMenu::setPyEscapeCallback(boost::python::object cb) {
    pyEscapeCallback_ = cb;
}

void GumpMenu::executePythonCallback(boost::python::object& cb) const {
    if (cb) {
        try {
            cb(boost::python::ptr(this));
        } catch (boost::python::error_already_set const&) {
            ui::Manager::getPythonLoader()->logError();
        }
    }
}

void GumpMenu::close() {
    ui::Manager::getSingleton()->closeGumpMenu(this);
}

int GumpMenu::getX() const {
    return get_geometry().left;
}

void GumpMenu::setX(int x) {
    CL_Rectf geom = get_geometry();
    int diff = x - geom.left;
    geom.translate(diff, 0);
    set_geometry(geom);
}

int GumpMenu::getY() const {
    return get_geometry().top;
}

void GumpMenu::setY(int y) {
    CL_Rectf geom = get_geometry();
    int diff = y - geom.top;
    geom.translate(0, diff);
    set_geometry(geom);
}

}
}
