
#include "gumpview.hpp"

#include <misc/log.hpp>

#include <world/manager.hpp>
#include <world/sectormanager.hpp>
#include <world/lightmanager.hpp>
#include <world/map.hpp>
#include <world/statics.hpp>
#include <world/ingameobject.hpp>

#include <ui/manager.hpp>
#include <ui/renderqueue.hpp>
#include <ui/doubleclickhandler.hpp>
#include <ui/cursormanager.hpp>
#include <ui/render/gumprenderer.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>

#include <algorithm>
#include <ClanLib/Display/Window/keys.h>

namespace fluo {
namespace ui {

GumpView::GumpView(CL_GUIComponent* parent, const CL_Rect& bounds) : GumpElement(parent) {
    this->set_geometry(bounds);
    renderer_.reset(new GumpRenderer(ui::Manager::getWorldRenderQueue(), this));

    set_constant_repaint(true);

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
    gc.push_cliprect(get_geometry());
    //CL_Draw::texture(gc, *renderer_->getTexture(gc)->getTexture(), CL_Rectf(0, 0, CL_Sizef(getWidth(), getHeight())));
    renderer_->render(gc);
    gc.pop_cliprect();
}

bool GumpView::onInputPressed(const CL_InputEvent& e) {
    bool consumed = true;

    boost::shared_ptr<world::IngameObject> clickedObject;

    //LOGARG_INFO(LOGTYPE_INPUT, "input pressed WorldView: %u", e.id);

    switch (e.id) {
    case CL_MOUSE_LEFT:
        set_focus();
        clickedObject = getFirstIngameObjectAt(e.mouse_pos.x, e.mouse_pos.y);
        if (clickedObject && clickedObject->isDraggable()) {
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
        clickedObject = getFirstIngameObjectAt(e.mouse_pos.x, e.mouse_pos.y);
        draggedObject = ui::Manager::getSingleton()->getCursorManager()->stopDragging();

        if (clickedObject) {
            if (draggedObject) {
                draggedObject->onDraggedOnto(clickedObject);
            } else {
                ui::Manager::getDoubleClickHandler()->notify(clickedObject);
            }
        } else if (draggedObject) {
            // dragged to void
            draggedObject->onDraggedToVoid();
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
        boost::shared_ptr<world::IngameObject> clickedObject = getFirstIngameObjectAt(e.mouse_pos.x, e.mouse_pos.y);
        if (!clickedObject) {
            LOG_DEBUG << "doublelicked, but found no object" << std::endl;
        } else {
            clickedObject->onClick();
        }

        return true;
    }

    return false;
}

boost::shared_ptr<world::IngameObject> GumpView::getFirstIngameObjectAt(unsigned int pixelX, unsigned int pixelY) {
    //LOG_INFO << "GumpView::getFirstObjectAt " << pixelX << " " << pixelY << std::endl;

    return renderer_->getRenderQueue()->getFirstObjectAt(pixelX, pixelY, false);
}

void GumpView::addObject(boost::shared_ptr<world::IngameObject> obj) {
    obj->addToRenderQueue(renderer_->getRenderQueue());
}

void GumpView::removeObject(boost::shared_ptr<world::IngameObject> obj) {
    obj->removeFromRenderQueue(renderer_->getRenderQueue());
}

}
}
