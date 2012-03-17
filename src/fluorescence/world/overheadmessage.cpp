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



#include "overheadmessage.hpp"

#include "manager.hpp"

#include <ui/texture.hpp>
#include <ui/manager.hpp>
#include <ui/fontengine.hpp>

#include <misc/log.hpp>

#include "mobile.hpp"
#include "manager.hpp"
#include "sectormanager.hpp"
#include "sector.hpp"

namespace fluo {
namespace world {

OverheadMessage::OverheadMessage(const UnicodeString& text, unsigned int font, unsigned int color, bool useRgbColor) :
            IngameObject(IngameObject::TYPE_SPEECH), milliSecondsToLive_(5000) {
    texture_ = ui::Manager::getFontEngine()->getUniFontTexture(font, text, 180, color, useRgbColor);
}

boost::shared_ptr<ui::Texture> OverheadMessage::getIngameTexture() const {
    return texture_;
}

void OverheadMessage::updateVertexCoordinates() {
    CL_Vec2f parentCoords = parentObject_.lock()->getVertexCoordinates()[0];

    int x = parentCoords.x + 22 - texture_->getWidth()/2;
    int y = parentCoords.y + parentPixelOffsetY_;

    CL_Rectf rect(x, y, CL_Sizef(texture_->getWidth(), texture_->getHeight()));

    worldRenderData_.setVertexCoordinates(rect);
}

void OverheadMessage::updateRenderDepth() {
    // Move to front
    // TODO: Handle mouse over
    boost::shared_ptr<Mobile> parent = boost::dynamic_pointer_cast<Mobile>(parentObject_.lock());

    int8_t z = parent->getLocZGame() + 7;

    worldRenderData_.setRenderDepth(parent->getLocXGame() + 40, parent->getLocYGame(), z, 50, 0, 0);
}

void OverheadMessage::updateTextureProvider() {
    // do nothing
}

bool OverheadMessage::updateAnimation(unsigned int elapsedMillis) {
    milliSecondsToLive_ -= elapsedMillis;

    return false;
}

bool OverheadMessage::isExpired() const {
    return milliSecondsToLive_ <= 0;
}

void OverheadMessage::expire() {
    boost::shared_ptr<IngameObject> parent = parentObject_.lock();
    if (parent) {
        parent->removeChildObject(shared_from_this());
    } else {
        removeFromAllRenderQueues();
    }

    boost::shared_ptr<OverheadMessage> sharedThis = boost::static_pointer_cast<OverheadMessage>(shared_from_this());
    world::Manager::getSingleton()->unregisterOverheadMessage(sharedThis);
    
    if (sector_) {
        sector_->removeDynamicObject(this);
    }
    
    sector_.reset();
}

void OverheadMessage::onClick() {
    LOG_DEBUG << "Overhead message clicked" << std::endl;
}

void OverheadMessage::setParentPixelOffset(int y) {
    parentPixelOffsetY_ = y;

    invalidateVertexCoordinates();
}

void OverheadMessage::onAddedToParent() {
    boost::shared_ptr<IngameObject> parent = parentObject_.lock();

    if (parent) {
        std::list<boost::shared_ptr<ui::RenderQueue> >::iterator iter = parent->rqBegin();
        std::list<boost::shared_ptr<ui::RenderQueue> >::iterator end = parent->rqEnd();
        for (; iter != end; ++iter) {
            addToRenderQueue(*iter);
        }
    }
}

void OverheadMessage::onRemovedFromParent() {
    boost::shared_ptr<IngameObject> parent = parentObject_.lock();

    // remove this item from all render queues the parent is in
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator iter = rqBegin();
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator end = rqEnd();

    std::list<boost::shared_ptr<ui::RenderQueue> > rqsToRemove;
    for (; iter != end; ++iter) {
        if (parent->isInRenderQueue(*iter)) {
            rqsToRemove.push_back(*iter);
        }
    }

    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator remIter = rqsToRemove.begin();
    std::list<boost::shared_ptr<ui::RenderQueue> >::iterator remEnd = rqsToRemove.end();
    for (; remIter != remEnd; ++remIter) {
        removeFromRenderQueue(*remIter);
    }
}

void OverheadMessage::onLocationChanged(const CL_Vec3f& oldLocation) {
    boost::shared_ptr<Sector> newSector = world::Manager::getSectorManager()->getSectorForCoordinates(getLocXGame(), getLocYGame());
    
    if (sector_ != newSector) {
        if (sector_) {
            sector_->removeDynamicObject(this);
        }
        
        if (newSector) {
            newSector->addDynamicObject(this);
        }
    }
    
    sector_ = newSector;
}

}
}
