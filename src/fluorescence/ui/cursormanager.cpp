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

#include <ClanLib/Display/2D/draw.h>
#include <ClanLib/Display/Render/texture.h>
#include <ClanLib/Core/Math/quad.h>

#include <client.hpp>
#include <misc/config.hpp>
#include <misc/log.hpp>

#include "texture.hpp"
#include "targeting/target.hpp"

namespace fluo {
namespace ui {

CursorManager::CursorManager(Config& config, boost::shared_ptr<CL_DisplayWindow> window) :
        isDragging_(false), enableFlags_(CursorEnableFlags::NONE), warMode_(false),
        cursorDirection_(CursorType::GAME_WEST), cursorOverride_(0xFFFFFFFFu) {

    unsigned int artIdStart = config["/fluo/ui/cursor@normal-artid-start"].asInt();
    unsigned int artIdStartWarMode = config["/fluo/ui/cursor@warmode-artid-start"].asInt();

    for (unsigned int i = 0; i < CursorType::COUNT; ++i) {
        setCursorImage(i, artIdStart + i, false, window);
        setCursorImage(i, artIdStartWarMode + i, true, window);
    }

    updateCursor();
}

void CursorManager::setCursorDirection(unsigned int direction) {
    // the direction enum is a bit different than the directions for the cursors
    // in Direction, N=0x0. thank you, osi ...
    cursorDirection_ = direction & 0x7;
    cursorDirection_  = (cursorDirection_ + 1) % 8;
    updateCursor();
}

void CursorManager::setWarMode(bool value) {
    warMode_ = value;
    updateCursor();
}

void CursorManager::resetCursorOverride() {
    cursorOverride_ = 0xFFFFFFFFu;
    updateCursor();
}

void CursorManager::setCursorOverride(unsigned int id) {
    cursorOverride_ = id;
    updateCursor();
}

void CursorManager::setCursorImage(unsigned int id, unsigned int artId, bool warMode, boost::shared_ptr<CL_DisplayWindow> window) {
    if (warMode) {
        id += CursorType::COUNT;
    }
    cursorImages_[id].set(id, artId, window);
}

void CursorManager::updateCursor() {
    unsigned int cursorId = CursorType::GAME_WEST;
    if ((enableFlags_ & CursorEnableFlags::OVERRIDE) && cursorOverride_ != 0xFFFFFFFFu) {
        cursorId = cursorOverride_;
    } else if (hasTarget()) {
        cursorId = CursorType::TARGET;
    } else if (enableFlags_ & CursorEnableFlags::DIRECTION) {
        cursorId = cursorDirection_;
    }

    if (warMode_) {
        cursorId += CursorType::COUNT;
    }

    //LOG_DEBUG << "NEW CURSOR ID: " << cursorId
            //<< " enable: " << std::hex << enableFlags_ << std::dec
            //<< " dir: " << cursorDirection_
            //<< " override: " << cursorOverride_
            //<< " target?: " << hasTarget()
            //<< " warmode?: " << warMode_
            //<< std::endl;
    cursorImages_[cursorId].activate();
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

        updateCursor();
    }
}

void CursorManager::setTarget(boost::shared_ptr<targeting::Target> targ) {
    // should we cancel the previous target here? seems like a logical thing to do, but
    // if both the last and current target were induced by the server, runuo interprets the
    // cancelling of the old one wrong (= as cancelling the new one).
    // thus, do not cancel here - for now
    target_ = targ;
    updateCursor();
}

bool CursorManager::hasTarget() const {
    return (bool)target_ && (enableFlags_ & CursorEnableFlags::TARGET);
}

bool CursorManager::onTarget(boost::shared_ptr<world::IngameObject> obj) {
    if (!hasTarget()) {
        return false;
    }

    if (target_->acceptsTarget(obj)) {
        target_->onTarget(obj);
        target_.reset();
        updateCursor();

        return true;
    } else {
        return false;
    }
}

void CursorManager::setCursorEnableFlags(unsigned int flags) {
    enableFlags_ = flags;
    updateCursor();
}

void CursorManager::releaseIngameObjects() {
    cancelTarget();
    dragCandidate_.reset();
    updateCursor();
}

void CursorManager::drawDragObject(CL_GraphicContext& gc, const CL_Point& mousePos) const {
    if (isDragging_ && dragCandidate_) {
        boost::shared_ptr<ui::Texture> dragTex = dragCandidate_->getIngameTexture();
        if (dragTex && dragTex->isReadComplete()) {
            // TODO: load shader here for correct hueing
            CL_Draw::texture(gc, dragTex->getTexture(),
                    CL_Quadf(CL_Rectf(mousePos.x - dragTex->getWidth() / 2, mousePos.y - dragTex->getHeight() / 2, CL_Sizef(dragTex->getWidth(), dragTex->getHeight()))),
                    CL_Colorf::white, dragTex->getNormalizedTextureCoords()
            );
        } else {
            dragCandidate_->updateRenderData(0);
        }
    }
}

}
}
