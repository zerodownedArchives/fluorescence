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



#include "cursormanager.hpp"

#include <client.hpp>
#include <misc/config.hpp>
#include <misc/log.hpp>

#include "targeting/target.hpp"

namespace fluo {
namespace ui {

CursorManager::CursorManager(Config& config, boost::shared_ptr<CL_DisplayWindow> window) :
        warMode_(false), isDragging_(false) {

    unsigned int artIdStart = config["/fluo/ui/cursor@normal-artid-start"].asInt();
    unsigned int artIdStartWarMode = config["/fluo/ui/cursor@warmode-artid-start"].asInt();

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
        int pixelDiffDrag = Client::getSingleton()->getConfig()["/fluo/input/mouse@drag-start-distance"].asInt();

        if (abs(mousePos.x - dragStartMouseX_) + abs(mousePos.y - dragStartMouseY_) >= pixelDiffDrag) {
            startDragging();
            dragCandidate_->onStartDrag(mousePos);
        }
    }
}

void CursorManager::cancelTarget() {
    if (target_) {
        target_->onCancel();
        target_.reset();

        setCursor(CursorType::GAME_WEST);
    }
}

void CursorManager::setTarget(boost::shared_ptr<targeting::Target> targ) {
    // should we cancel the previous target here? seems like a logical thing to do, but
    // if both the last and current target were induced by the server, runuo interprets the
    // cancelling of the old one wrong (= as cancelling the new one).
    // thus, do not cancel here - for now
    target_ = targ;
    setCursor(CursorType::TARGET);
}

bool CursorManager::hasTarget() const {
    return (bool)target_;
}

bool CursorManager::onTarget(boost::shared_ptr<world::IngameObject> obj) {
    if (!hasTarget()) {
        return false;
    }

    if (target_->acceptsTarget(obj)) {
        target_->onTarget(obj);
        target_.reset();
        setCursor(CursorType::GAME_WEST);

        return true;
    } else {
        return false;
    }
}

}
}
