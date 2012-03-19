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

#include "osieffect.hpp"

#include <data/manager.hpp>
#include <data/animdataloader.hpp>

namespace fluo {
namespace world {

OsiEffect::OsiEffect(unsigned int artId) : 
        Effect(IngameObject::TYPE_OSI_EFFECT),
        artId_(artId) {
}

boost::shared_ptr<ui::Texture> OsiEffect::getIngameTexture() const {
    return textureProvider_->getTexture();
}

void OsiEffect::updateTextureProvider() {
    textureProvider_ = data::Manager::getItemTextureProvider(artId_);
}

bool OsiEffect::updateAnimation(unsigned int elapsedMillis) {
    return textureProvider_->update(elapsedMillis);
}

void OsiEffect::updateVertexCoordinates() {
    int texWidth = getIngameTexture()->getWidth();
    int texHeight = getIngameTexture()->getHeight();

    int px = (getLocXDraw() - getLocYDraw()) * 22 - texWidth/2 + 22;
    int py = (getLocXDraw() + getLocYDraw()) * 22 - texHeight + 44;
    py -= getLocZDraw() * 4;
    
    if (fixedAngle_) {
        CL_Rectf rect(px, py, px + texWidth, py + texHeight);
        worldRenderData_.setVertexCoordinates(rect);
    } else {
        float centerX = px + texWidth / 2.0f;
        float centerY = py + texHeight / 2.0f;
        
        float sinCur = sin(currentAngle_);
        float cosCur = cos(currentAngle_);
        
        float tmpX = px - centerX;
        float tmpY = py - centerY;
        worldRenderData_.setVertexCoordinates(0, (centerX + tmpX * cosCur - tmpY * sinCur), (centerY + tmpX * sinCur + tmpY * cosCur));
        
        tmpX = px + texWidth - centerX;
        worldRenderData_.setVertexCoordinates(1, (centerX + tmpX * cosCur - tmpY * sinCur), (centerY + tmpX * sinCur + tmpY * cosCur));
        worldRenderData_.setVertexCoordinates(3, (centerX + tmpX * cosCur - tmpY * sinCur), (centerY + tmpX * sinCur + tmpY * cosCur));
        
        tmpY = py + texHeight - centerY;
        worldRenderData_.setVertexCoordinates(5, (centerX + tmpX * cosCur - tmpY * sinCur), (centerY + tmpX * sinCur + tmpY * cosCur));
        
        tmpX = px - centerX;
        worldRenderData_.setVertexCoordinates(2, (centerX + tmpX * cosCur - tmpY * sinCur), (centerY + tmpX * sinCur + tmpY * cosCur));
        worldRenderData_.setVertexCoordinates(4, (centerX + tmpX * cosCur - tmpY * sinCur), (centerY + tmpX * sinCur + tmpY * cosCur));
    }
}

void OsiEffect::updateRenderDepth() {
    worldRenderData_.setRenderDepth(getLocXGame(), getLocYGame(), getLocZGame() + 7, 50, 0, 0);
}

unsigned int OsiEffect::startExplosion() {
    switch (rand() % 3) {
        default:
        case 0: artId_ = 0x36B0; break;
        case 1: artId_ = 0x36BD; break;
        case 2: artId_ = 0x36CB; break;
    }
    
    invalidateTextureProvider();
    data::AnimDataInfo explosionInfo = data::Manager::getAnimDataLoader()->getInfo(artId_);
    return explosionInfo.frameCount_ * explosionInfo.frameIntervalMillis_;
}

}
}

