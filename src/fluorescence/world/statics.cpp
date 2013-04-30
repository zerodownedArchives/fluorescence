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



#include "statics.hpp"

#include <client.hpp>
#include <misc/config.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/tiledataloader.hpp>
#include <data/huesloader.hpp>
#include <data/radarcolloader.hpp>

#include <ui/texture.hpp>
#include <ui/manager.hpp>
#include <ui/render/material.hpp>

#include "sector.hpp"

namespace fluo {
namespace world {

StaticItem::StaticItem() : IngameObject(IngameObject::TYPE_STATIC_ITEM) {
}

boost::shared_ptr<ui::Texture> StaticItem::getIngameTexture() const {
    return textureProvider_->getTexture();
}

unsigned int StaticItem::getArtId() const {
    return artId_;
}

void StaticItem::set(int locX, int locY, int locZ, unsigned int artId, unsigned int hue) {
    artId_ = artId;
    hue_ = hue;
    tileDataInfo_ = data::Manager::getTileDataLoader()->getStaticTileInfo(artId_);

    setLocation(locX, locY, locZ);

    worldRenderData_.hueInfo_[0u] = tileDataInfo_->partialHue() ? 1.0 : 0.0;
    worldRenderData_.hueInfo_[1u] = data::Manager::getHuesLoader()->translateHue(hue_);
    worldRenderData_.hueInfo_[2u] = tileDataInfo_->translucent() ? 0.8 : 1.0;

    setIgnored(ui::Manager::isStaticIdIgnored(artId_));
    if (ui::Manager::isStaticIdWater(artId_)) {
        setMaterial(Material::WATER);
    }
}

void StaticItem::updateVertexCoordinates() {
    int texWidth = getIngameTexture()->getWidth();
    int texHeight = getIngameTexture()->getHeight();

    int px = (getLocXDraw() - getLocYDraw()) * 22 - texWidth/2 + 22;
    int py = (getLocXDraw() + getLocYDraw()) * 22 - texHeight + 44;
    py -= getLocZDraw() * 4;

    CL_Rectf rect(px, py, px + texWidth, py + texHeight);

    worldRenderData_.setVertexCoordinates(rect);
}

void StaticItem::updateRenderDepth() {
    // render prio

    // level 1 z and tiledata flags
    int8_t z = getLocZGame();
    if (tileDataInfo_->background() && tileDataInfo_->surface()) {
        z += 4;
    } else if (tileDataInfo_->background()) {
        z += 2;
    } else if (tileDataInfo_->surface()) {
        z += 5;
    } else {
        z += 6;
    }

    worldRenderData_.setRenderDepth(getLocXGame(), getLocYGame(), z, 10, tileDataInfo_->height_, indexInBlock_);
}

void StaticItem::updateTextureProvider() {
    textureProvider_ = data::Manager::getItemTextureProvider(artId_);
}

bool StaticItem::updateAnimation(unsigned int elapsedMillis) {
    return textureProvider_->update(elapsedMillis) || materialInfo_->constantRepaint_;
}

const data::StaticTileInfo* StaticItem::getTileDataInfo() const {
    return tileDataInfo_;
}

void StaticItem::onClick() {
    LOG_INFO << "Clicked static, id=" << std::hex << getArtId() << std::dec << " loc=(" << getLocXGame() << "/" << getLocYGame() << "/" <<
            getLocZGame() << ") name=" << " hue=" << hue_ << " " << tileDataInfo_->name_ << std::endl;

    LOG_INFO << "impassable=" << tileDataInfo_->impassable() << " surface=" << tileDataInfo_->surface() << " bridge=" << tileDataInfo_->bridge() << " height=" << (unsigned int)tileDataInfo_->height_ << std::endl;

    //LOG_INFO << "background=" << tileDataInfo_->background() << " surface=" << tileDataInfo_->surface() << " height=" << (unsigned int)tileDataInfo_->height_ <<
            //" hue=" << hue_ << " indexInBlock=" << indexInBlock_ << std::endl;

    if (sector_) {
        LOG_DEBUG << "sector id=" << sector_->getSectorId().value_ << std::endl;
    }

    printRenderDepth();
}

bool StaticItem::periodicRenderUpdateRequired() const {
    return tileDataInfo_->animation();
}


StaticBlock::StaticBlock() :
        blockIndexX_(0), blockIndexY_(0),
        rawData_(nullptr), rawBlockCount_(0) {
    memset(miniMapPixels_, 0, 64 * 4);
    memset(miniMapHeights_, (int8_t)-127, 64);
}

StaticBlock::~StaticBlock() {
    if (rawData_) {
        free(rawData_);
        rawBlockCount_ = 0;
    }
}

void StaticBlock::setIndex(unsigned int x, unsigned int y) {
    blockIndexX_ = x;
    blockIndexY_ = y;
}

std::list<boost::shared_ptr<StaticItem> >& StaticBlock::getItemList() {
    return itemList_;
}

void StaticBlock::setRawData(const int8_t* data, unsigned int len) {
    rawBlockCount_ = len / 7;
    rawData_ = (RawStaticsBlock*)malloc(rawBlockCount_ * 7);
    memcpy(rawData_, data, len);
}

void StaticBlock::generateItemsFromRawData() {
    unsigned int cellOffsetX = blockIndexX_ * 8;
    unsigned int cellOffsetY = blockIndexY_ * 8;

    RawStaticsBlock* rawBlock = rawData_;

    for (unsigned int i = 0; i < rawBlockCount_; ++i) {
        boost::shared_ptr<world::StaticItem> cur(new world::StaticItem);
        cur->indexInBlock_ = i;
        cur->set(cellOffsetX + rawBlock->cellX_, cellOffsetY + rawBlock->cellY_, rawBlock->cellZ_, rawBlock->artId_, rawBlock->hue_);
        itemList_.push_back(cur);

        ++rawBlock;
    }
}

void StaticBlock::generateMiniMap() {
    RawStaticsBlock* rawBlock = rawData_;

    data::RadarColLoader* radarCol = data::Manager::getRadarColLoader().get();
    for (unsigned int i = 0; i < rawBlockCount_; ++i) {
        if (rawBlock->cellZ_ >= miniMapHeights_[rawBlock->cellY_ * 8 + rawBlock->cellX_]) {
            miniMapPixels_[rawBlock->cellY_ * 8 + rawBlock->cellX_] = radarCol->getStaticColor(rawBlock->artId_);
            miniMapHeights_[rawBlock->cellY_ * 8 + rawBlock->cellX_] = rawBlock->cellZ_;
        }

        ++rawBlock;
    }
}

const uint32_t* StaticBlock::getMiniMapPixels() const {
    return miniMapPixels_;
}

const int8_t * StaticBlock::getMiniMapHeight() const {
    return miniMapHeights_;
}

void StaticBlock::dropItems() {
    itemList_.clear();
}

}
}
