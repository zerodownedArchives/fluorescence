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



#include "containerview.hpp"

#include <misc/log.hpp>

#include <world/ingameobject.hpp>
#include <world/dynamicitem.hpp>

#include <ui/manager.hpp>
#include <ui/cursormanager.hpp>
#include <ui/render/containerrenderer.hpp>
#include <ui/render/containerrenderqueue.hpp>

#include <data/manager.hpp>
#include <data/gumpartloader.hpp>

#include <ClanLib/Display/Window/keys.h>

namespace fluo {
namespace ui {
namespace components {

ContainerView::ContainerView(CL_GUIComponent* parent, const CL_Rect& bounds) : GumpElement(parent), sizeAdjusted(false) {
    this->set_geometry(bounds);
    boost::shared_ptr<ContainerRenderQueue> rq(new ContainerRenderQueue());
    renderer_.reset(new ContainerRenderer(rq, this));

    func_render().set(this, &ContainerView::renderOneFrame);
    func_input_pressed().set(this, &ContainerView::onInputPressed);
    func_input_released().set(this, &ContainerView::onInputReleased);
    func_input_doubleclick().set(this, &ContainerView::onDoubleClick);
    func_pointer_enter().set(this, &ContainerView::onPointerEnter);
    func_pointer_exit().set(this, &ContainerView::onPointerExit);
}

ContainerView::~ContainerView() {
    if (containerObject_) {
        containerObject_->onContainerGumpClosed();
    }
}

unsigned int ContainerView::getWidth() {
    return get_width();
}

unsigned int ContainerView::getHeight() {
    return get_height();
}

void ContainerView::renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    if (!sizeAdjusted && backgroundTexture_ && backgroundTexture_->isReadComplete()) {
        sizeAdjusted = true;

        renderer_->getRenderQueue()->forceRepaint();
    }

    CL_Draw::texture(gc, renderer_->getTexture(gc), CL_Rectf(0, 0, CL_Sizef(getWidth(), getHeight())));
}

bool ContainerView::onInputPressed(const CL_InputEvent& e) {
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

bool ContainerView::onInputReleased(const CL_InputEvent& e) {
    bool consumed = true;

    boost::shared_ptr<world::IngameObject> clickedObject;
    boost::shared_ptr<world::IngameObject> draggedObject;

    switch (e.id) {
    case CL_MOUSE_LEFT:
        clickedObject = renderer_->getRenderQueue()->getFirstObjectAt(e.mouse_pos.x, e.mouse_pos.y, false);
        draggedObject = ui::Manager::getSingleton()->getCursorManager()->stopDragging();

        if (clickedObject) {
            if (draggedObject) {
                ui::Manager::getSingleton()->onDragDrop(draggedObject, clickedObject);
            } else {
                ui::Manager::getSingleton()->onSingleClick(clickedObject);
            }
        } else if (draggedObject) {
            // dragged to void
            boost::shared_ptr<world::IngameObject> nullPtr;
            ui::Manager::getSingleton()->onDragDrop(draggedObject, nullPtr);
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

bool ContainerView::onDoubleClick(const CL_InputEvent& e) {
    if (e.id == CL_MOUSE_LEFT) {
        boost::shared_ptr<world::IngameObject> clickedObject = renderer_->getRenderQueue()->getFirstObjectAt(e.mouse_pos.x, e.mouse_pos.y, false);
        if (!clickedObject) {
            LOG_DEBUG << "doublelicked, but found no object" << std::endl;
        } else {
            ui::Manager::getSingleton()->onDoubleClick(clickedObject);
        }

        return true;
    }

    return false;
}

void ContainerView::addObject(boost::shared_ptr<world::IngameObject> obj) {
    obj->addToRenderQueue(renderer_->getRenderQueue());
    request_repaint();
}

void ContainerView::removeObject(boost::shared_ptr<world::IngameObject> obj) {
    obj->removeFromRenderQueue(renderer_->getRenderQueue());
    request_repaint();
}

void ContainerView::setBackgroundGumpId(unsigned int gumpId) {
    backgroundTexture_ = data::Manager::getGumpArtLoader()->getTexture(gumpId);
    sizeAdjusted = false;
    request_repaint();
}

boost::shared_ptr<ui::Texture> ContainerView::getBackgroundTexture() {
    return backgroundTexture_;
}

void ContainerView::setContainerObject(boost::shared_ptr<world::DynamicItem> cont) {
    containerObject_ = cont;
}

bool ContainerView::onPointerEnter() {
    ui::Manager::getCursorManager()->setCursorEnableFlags(CursorEnableFlags::TARGET);
    return true;
}

bool ContainerView::onPointerExit() {
    ui::Manager::getCursorManager()->setCursorEnableFlags(CursorEnableFlags::NONE);
    return true;
}

bool ContainerView::has_pixel(const CL_Point& p) const {
    return backgroundTexture_ && backgroundTexture_->isReadComplete() && backgroundTexture_->hasPixel(p.x, p.y);
}

}
}
}
