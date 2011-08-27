
#include "cursormanager.hpp"

#include <client.hpp>
#include <misc/config.hpp>
#include <misc/log.hpp>

namespace uome {
namespace ui {

CursorManager::CursorManager(Config& config, boost::shared_ptr<CL_DisplayWindow> window) :
        warMode_(false), isDragging_(false) {

    unsigned int artIdStart = config["/uome/ui/cursor@normal-artid-start"].asInt();
    unsigned int artIdStartWarMode = config["/uome/ui/cursor@warmode-artid-start"].asInt();

    for (unsigned int i = 0; i < CursorType::COUNT; ++i) {
        setCursorImage(i, artIdStart + i, false, window);
        setCursorImage(i, artIdStartWarMode + i, true, window);
    }
}

void CursorManager::setWarMode(bool value) {
    warMode_ = value;
    updateCursor();
}

void CursorManager::setCursor(unsigned int id) {
    currentCursorId_ = id;
    updateCursor();
}

void CursorManager::setCursorImage(unsigned int id, unsigned int artId, bool warMode, boost::shared_ptr<CL_DisplayWindow> window) {
    if (warMode) {
        id += CursorType::COUNT;
    }
    cursorImages_[id].set(id, artId, window);
}

void CursorManager::updateCursor() {
    unsigned int cursorIndex = currentCursorId_;
    if (warMode_) {
        cursorIndex += CursorType::COUNT;
    }

    cursorImages_[cursorIndex].activate();
}

void CursorManager::setDragCandidate(boost::shared_ptr<world::IngameObject> itm, int mouseX, int mouseY) {
    if (!isDragging_) {
        dragCandidate_ = itm;
        dragStartMouseX_ = mouseX;
        dragStartMouseY_ = mouseY;
    }
}

void CursorManager::startDragging() {
    if (dragCandidate_ && !isDragging_) {
        isDragging_ = true;
        LOG_DEBUG << "start dragging" << std::endl;
    }
}

boost::shared_ptr<world::IngameObject> CursorManager::stopDragging() {
    boost::shared_ptr<world::IngameObject> ret;
    if (isDragging_ && dragCandidate_) {
        ret = dragCandidate_;
    }
    isDragging_ = false;
    dragCandidate_.reset();
    return ret;
}

bool CursorManager::isDragging() const {
    return isDragging_;
}

void CursorManager::onCursorMove(const CL_Point& mousePos) {
    if (dragCandidate_ && !isDragging_) {
        int pixelDiffDrag = Client::getSingleton()->getConfig()["/uome/input/mouse@drag-start-distance"].asInt();

        if (abs(mousePos.x - dragStartMouseX_) + abs(mousePos.y - dragStartMouseY_) >= pixelDiffDrag) {
            startDragging();
            dragCandidate_->onStartDrag(mousePos);
        }
    }
}

}
}
