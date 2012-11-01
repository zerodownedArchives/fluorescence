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


#ifndef FLUO_UI_GUMPMENU_HPP
#define FLUO_UI_GUMPMENU_HPP

#include <ClanLib/GUI/Components/window.h>
#include <ClanLib/Display/Font/font.h>
#include <map>
#include <boost/function.hpp>
#include <boost/python/dict.hpp>

#include <typedefs.hpp>
#include <misc/config.hpp>

#include "gumpmenus.hpp"
#include "gumpcomponent.hpp"

namespace fluo {
namespace world {
    class Mobile;
}

namespace ui {

class GumpMenu : public CL_Window {
public:
    GumpMenu(const CL_GUITopLevelDescription& desc);
    ~GumpMenu();

    // add/remove components
    void removeFromPages(const GumpComponent* component);
    void addToCurrentPage(GumpComponent* component);

    // page management
    void addPage(unsigned int pageId);

    void activatePage(unsigned int pageId);
    void activateFirstPage();

    unsigned int getActivePageId();


    // basic properties
    void setClosable(bool value);
    bool isClosable();

    void setDraggable(bool value);
    bool isDraggable();

    void setName(const UnicodeString& name);
    const UnicodeString& getName();

    void setSerial(Serial serial);
    void setTypeId(unsigned int typeId);
    Serial getSerial() const;
    unsigned int getTypeId() const;

    void setCurrentRadioGroup(unsigned int groupId);
    unsigned int getCurrentRadioGroup() const;

    void setFont(const UnicodeString& name, unsigned int height);
    void setFontB(const UnicodeString& name, unsigned int height, bool border);
    CL_Font getFont() const;

    // mobile linking
    void updateMobileProperties();
    void setLinkedMobile(world::Mobile* mob);
    world::Mobile* getLinkedMobile() const;


    // event stuff
    void onClose();
    void setCloseCallback(boost::function<void()> cb);
    void sendReply(unsigned int buttonId_);

    // auto-resize
    void fitSizeToChildren();


    virtual bool has_pixel(const CL_Point& p) const;
    void startDragging(const CL_Point& mousePos);

    GumpComponent* getNamedComponent(const UnicodeString& name);

    void close();


    // python specific
    static GumpMenu* create(const UnicodeString& name, int x, int y);
    static GumpMenu* createBackground(const UnicodeString& name, int x, int y, bool inBackground);

    boost::python::dict getPythonStore();

    boost::python::object getPyEnterCallback() const;
    void setPyEnterCallback(boost::python::object cb);
    boost::python::object getPyEscapeCallback() const;
    void setPyEscapeCallback(boost::python::object cb);

private:
    Serial serial_;
    unsigned int typeId_;

    unsigned int activePageId_;
    std::map<unsigned int, std::vector<GumpComponent*> > pages_;
    unsigned int firstPageId_;

    void internalActivatePage(unsigned int pageId);
    void internalDeactivatePage(unsigned int pageId);

    bool closable_;

    bool draggable_;
    bool isDragged_;
    CL_Point lastMousePos_;

    bool onInputPressed(const CL_InputEvent& msg);
    bool onInputReleased(const CL_InputEvent& msg);
    bool onPointerMoved(const CL_InputEvent& msg);

    UnicodeString name_;

    world::Mobile* linkedMobile_;
    void updateMobilePropertiesRec(CL_GUIComponent* comp);

    unsigned int currentRadioGroup_;

    boost::function<void()> closeCallback_;

    CL_FontDescription fontDesc_;
    CL_Font cachedFont_;

    boost::python::dict pythonStore_;
    boost::python::object pyEnterCallback_;
    boost::python::object pyEscapeCallback_;

    void executePythonCallback(boost::python::object& cb) const;
};

}
}

#endif
