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



#include "gumpview.hpp"

#include <misc/log.hpp>

#include <world/manager.hpp>
#include <world/sectormanager.hpp>
#include <world/lightmanager.hpp>
#include <world/map.hpp>
#include <world/statics.hpp>
#include <world/ingameobject.hpp>

#include <ui/manager.hpp>
#include <ui/render/renderqueue.hpp>
#include <ui/doubleclickhandler.hpp>
#include <ui/cursormanager.hpp>
#include <ui/render/gumprenderer.hpp>
#include <ui/render/gumprenderqueue.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>

#include <algorithm>
#include <ClanLib/Display/Window/keys.h>

namespace fluo {
namespace ui {
namespace components {

GumpView::GumpView(CL_GUIComponent* parent, const CL_Rect& bounds) : GumpElement(parent) {
    this->set_geometry(bounds);
    boost::shared_ptr<GumpRenderQueue> rq(new GumpRenderQueue());
    renderer_.reset(new GumpRenderer(rq, this));

    func_render().set(this, &GumpView::renderOneFrame);
    func_input_pressed().set(this, &GumpView::onInputPressed);
    func_input_released().set(this, &GumpView::onInputReleased);
    func_input_doubleclick().set(this, &GumpView::onDoubleClick);
}

GumpView::~GumpView() {
}

unsigned int GumpView::getWidth() {
    return get_width();
}

unsigned int GumpView::getHeight() {
    return get_height();
}

void GumpView::renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    CL_Draw::texture(gc, renderer_->getTexture(gc), CL_Rectf(0, 0, CL_Sizef(getWidth(), getHeight())));
}

bool GumpView::onInputPressed(const CL_InputEvent& e) {
    bool consumed = true;

    boost::shared_ptr<world::IngameObject> clickedObject;

    //LOGARG_INFO(LOGTYPE_INPUT, "input pressed WorldView: %u", e.id);

    switch (e.id) {
    case CL_MOUSE_LEFT:
        set_focus();
        clickedObject = renderer_->getRenderQueue()->getFirstObjectAt(e.mouse_pos.x, e.mouse_pos.y, false);
        if (!clickedObject) {
            consumed = false;
        } else if (clickedObject->isDraggable()) {
            ui::Manager::getSingleton()->getCursorManager()->setDragCandidate(clickedObject, e.mouse_pos.x, e.mouse_pos.y);
        }
        break;

    default:
        consumed = false;
        break;
    }

    return consumed;
}

bool GumpView::onInputReleased(const CL_InputEvent& e) {
    bool consumed = true;

    boost::shared_ptr<world::IngameObject> clickedObject;
    boost::shared_ptr<world::IngameObject> draggedObject;

    switch (e.id) {
    case CL_MOUSE_LEFT:
        clickedObject = renderer_->getRenderQueue()->getFirstObjectAt(e.mouse_pos.x, e.mouse_pos.y, false);
        draggedObject = ui::Manager::getSingleton()->getCursorManager()->stopDragging();

        if (clickedObject) {
            if (draggedObject) {
                ui::Manager::getSingleton()->queueDrag(draggedObject, clickedObject);
            } else {
                ui::Manager::getSingleton()->onClickEvent(clickedObject);
            }
        } else if (draggedObject) {
            // dragged to void
            boost::shared_ptr<world::IngameObject> nullPtr;
            ui::Manager::getSingleton()->queueDrag(draggedObject, nullPtr);
        } else {
            consumed = false;
        }
        break;

    default:
        consumed = false;
        break;
    }

    return consumed;
}

bool GumpView::onDoubleClick(const CL_InputEvent& e) {
    if (e.id == CL_MOUSE_LEFT) {
        boost::shared_ptr<world::IngameObject> clickedObject = renderer_->getRenderQueue()->getFirstObjectAt(e.mouse_pos.x, e.mouse_pos.y, false);
        if (!clickedObject) {
            LOG_DEBUG << "doublelicked, but found no object" << std::endl;
        } else {
            clickedObject->onDoubleClick();
        }

        return true;
    }

    return false;
}

void GumpView::addObject(boost::shared_ptr<world::IngameObject> obj) {
    obj->addToRenderQueue(renderer_->getRenderQueue());
    request_repaint();
}

void GumpView::removeObject(boost::shared_ptr<world::IngameObject> obj) {
    obj->removeFromRenderQueue(renderer_->getRenderQueue());
    request_repaint();
}

}
}
}
