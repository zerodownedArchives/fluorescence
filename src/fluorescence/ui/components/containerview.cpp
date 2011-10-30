
#include "containerview.hpp"

#include <misc/log.hpp>

#include <world/ingameobject.hpp>
#include <world/dynamicitem.hpp>

#include <ui/manager.hpp>
#include <ui/doubleclickhandler.hpp>
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

    set_constant_repaint(true);

    func_render().set(this, &ContainerView::renderOneFrame);
    func_input_pressed().set(this, &ContainerView::onInputPressed);
    func_input_released().set(this, &ContainerView::onInputReleased);
    func_input_doubleclick().set(this, &ContainerView::onDoubleClick);
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
        //CL_GUIComponent* topComponent = get_top_level_component();
        //CL_Rectf topGeom = topComponent->get_geometry();
        //topGeom.set_size(CL_Sizef(backgroundTexture_->getWidth(), backgroundTexture_->getHeight()));
        //topComponent->set_geometry(topGeom);
        sizeAdjusted = true;

        renderer_->getRenderQueue()->forceRepaint();
    }

    CL_Draw::texture(gc, *renderer_->getTexture(gc)->getTexture(), CL_Rectf(0, 0, CL_Sizef(getWidth(), getHeight())));
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

bool ContainerView::onDoubleClick(const CL_InputEvent& e) {
    if (e.id == CL_MOUSE_LEFT) {
        boost::shared_ptr<world::IngameObject> clickedObject = renderer_->getRenderQueue()->getFirstObjectAt(e.mouse_pos.x, e.mouse_pos.y, false);
        if (!clickedObject) {
            LOG_DEBUG << "doublelicked, but found no object" << std::endl;
        } else {
            clickedObject->onClick();
        }

        return true;
    }

    return false;
}

void ContainerView::addObject(boost::shared_ptr<world::IngameObject> obj) {
    obj->addToRenderQueue(renderer_->getRenderQueue());
}

void ContainerView::removeObject(boost::shared_ptr<world::IngameObject> obj) {
    obj->removeFromRenderQueue(renderer_->getRenderQueue());
}

void ContainerView::setBackgroundGumpId(unsigned int gumpId) {
    backgroundTexture_ = data::Manager::getGumpArtLoader()->getTexture(gumpId);
    sizeAdjusted = false;
}

boost::shared_ptr<ui::Texture> ContainerView::getBackgroundTexture() {
    return backgroundTexture_;
}

void ContainerView::setContainerObject(boost::shared_ptr<world::DynamicItem> cont) {
    containerObject_ = cont;
}

}
}
}
