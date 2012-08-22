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



#include "sector.hpp"

#include "dynamicitem.hpp"

#include <data/manager.hpp>
#include <data/staticsloader.hpp>
#include <data/maploader.hpp>

#include <ui/manager.hpp>
#include <ui/cliprectmanager.hpp>
#include <ui/render/material.hpp>

namespace fluo {
namespace world {

Sector::Sector(unsigned int mapId, const IsoIndex& sectorId) :
        mapId_(mapId), id_(sectorId),
        mapAddedToList_(false), staticsAddedToList_(false),
        visible_(true), fullUpdateRenderDataRequired_(true), repaintRequired_(false) {

    //LOG_DEBUG << "Sector construct, map=" << mapId_ << " x=" << getLocX() << " y=" << getLocY() << std::endl;

    mapBlock_ = data::Manager::getMapLoader(mapId_)->get(getLocX(), getLocY());
    staticBlock_ = data::Manager::getStaticsLoader(mapId_)->get(getLocX(), getLocY());
}

Sector::~Sector() {
    //LOG_DEBUG << "Sector destruct, map=" << mapId_ << " x=" << getLocX() << " y=" << getLocY() << std::endl;
}

unsigned int Sector::getLocX() const {
    return id_.x_;
}
unsigned int Sector::getLocY() const {
    return id_.y_;
}

bool Sector::isVisible() const {
    return visible_;
}

unsigned int Sector::getMapId() const {
    return mapId_;
}

const IsoIndex& Sector::getSectorId() const {
    return id_;
}

void Sector::update(unsigned int elapsedMillis) {
    repaintRequired_ = false;

    if (!mapAddedToList_ && mapBlock_ && mapBlock_->isReadComplete()) {
        // map block is now loaded => add to list
        for (unsigned int x = 0; x < 8; ++x) {
            for (unsigned int y = 0; y < 8; ++y) {
                renderList_.push_back(mapBlock_->get(x, y).get());
            }
        }

        fullUpdateRenderDataRequired_ = true;
        mapAddedToList_ = true;
    }

    if (!staticsAddedToList_ && staticBlock_ && staticBlock_->isReadComplete()) {
        // static block is now loaded => add to list
        std::list<boost::shared_ptr<world::StaticItem> > staticList = staticBlock_->getItemList();
        std::list<boost::shared_ptr<world::StaticItem> >::const_iterator it = staticList.begin();
        std::list<boost::shared_ptr<world::StaticItem> >::const_iterator end = staticList.end();

        for (; it != end; ++it) {
            renderList_.push_back(it->get());
        }

        fullUpdateRenderDataRequired_ = true;
        staticsAddedToList_ = true;
    }

    if (mapBlock_->repaintRequested_) {
        // happens when the neighboring z values of a map block change

        for (unsigned int x = 0; x < 8; ++x) {
            for (unsigned int y = 0; y < 8; ++y) {
                mapBlock_->get(x, y)->updateRenderData(elapsedMillis);
                renderListSortRequired_ |= mapBlock_->get(x, y)->renderDepthChanged();
                renderListSortRequired_ |= mapBlock_->get(x, y)->textureOrVerticesChanged();
            }
        }

        mapBlock_->repaintRequested_ = false;
    }

    //LOG_DEBUG << "Sector::update " << id_ << std::endl;
    if (fullUpdateRenderDataRequired_) {
        // this is executed only a few times after the sector is loaded, as long as not all textures for the sector are loaded
        // as soon as every static and map tile is loaded, only the things in the quick update list are updated

        //LOG_DEBUG << "full update required" << std::endl;

        // do we need to update everything?
        bool curFullUpdateRequired = false;
        repaintRequired_ = true;

        if (mapAddedToList_) {
            for (unsigned int x = 0; x < 8; ++x) {
                for (unsigned int y = 0; y < 8; ++y) {
                    if (!mapBlock_->get(x, y)->getWorldRenderData().renderDataValid()) {
                        mapBlock_->get(x, y)->updateRenderData(elapsedMillis);
                        renderListSortRequired_ |= mapBlock_->get(x, y)->renderDepthChanged();
                        curFullUpdateRequired = true;
                    }
                }
            }
        }

        if (staticsAddedToList_) {
            std::list<boost::shared_ptr<world::StaticItem> >::iterator it = staticBlock_->getItemList().begin();
            std::list<boost::shared_ptr<world::StaticItem> >::iterator end = staticBlock_->getItemList().end();

            for (; it != end; ++it) {
                if (!(*it)->getWorldRenderData().renderDataValid()) {
                    if (!(*it)->getWorldRenderData().renderDataValid()) {
                        (*it)->updateRenderData(elapsedMillis);
                        renderListSortRequired_ |= (*it)->renderDepthChanged();
                        curFullUpdateRequired = true;
                    }
                }
            }
        }

        if (!curFullUpdateRequired) {
            // all the items in this sector are now fully loaded, including texture etc
            // to save time in the future, update only the items that are animated. store those in a list now

            //LOG_DEBUG << "full update not required anymore" << std::endl;
            quickRenderUpdateList_.clear();

            if (mapAddedToList_) {
                for (unsigned int x = 0; x < 8; ++x) {
                    for (unsigned int y = 0; y < 8; ++y) {
                        if (mapBlock_->get(x, y)->getMaterial()->constantRepaint_) {
                            quickRenderUpdateList_.push_back(mapBlock_->get(x, y).get());
                        }
                    }
                }
            }

            if (staticsAddedToList_) {
                std::list<boost::shared_ptr<world::StaticItem> >::iterator it = staticBlock_->getItemList().begin();
                std::list<boost::shared_ptr<world::StaticItem> >::iterator end = staticBlock_->getItemList().end();

                for (; it != end; ++it) {
                    if ((*it)->periodicRenderUpdateRequired() || (*it)->getMaterial()->constantRepaint_) {
                        quickRenderUpdateList_.push_back(it->get());
                    }
                }
            }

            fullUpdateRenderDataRequired_ = false;

            //LOG_DEBUG << "Items in quicklist: " << quickRenderUpdateList_.size() << std::endl;


            ui::Manager::getClipRectManager()->forceFullRepaint();
        }
    } else if (!quickRenderUpdateList_.empty()) {
        //LOG_DEBUG << "quick render update, size=" << quickRenderUpdateList_.size() << std::endl;
        // only call update on those few things that really need to be updated (e.g. animated statics)
        std::list<world::IngameObject*>::iterator iter = quickRenderUpdateList_.begin();
        std::list<world::IngameObject*>::iterator end = quickRenderUpdateList_.end();

        for (; iter != end; ++iter) {
            (*iter)->updateRenderData(elapsedMillis);
            bool depthChanged = (*iter)->renderDepthChanged();
            bool texOrVertChanged = (*iter)->textureOrVerticesChanged();
            renderListSortRequired_ |= depthChanged;
            repaintRequired_ |= texOrVertChanged;

            if (depthChanged || texOrVertChanged) {
                // add previous and current vertex coordinates to clipped update range
                (*iter)->repaintRectangle(true);
            }
        }
    }

    if (renderListSortRequired_) {
        renderListSortRequired_ = false;
        repaintRequired_ = true;
        renderList_.sort(&Sector::renderDepthSortHelper);
    }
}

bool Sector::renderDepthSortHelper(const world::IngameObject* a, const world::IngameObject* b) {
    return a->getRenderDepth() < b->getRenderDepth();
}

std::list<world::IngameObject*>::iterator Sector::renderBegin() {
    return renderList_.begin();
}

std::list<world::IngameObject*>::iterator Sector::renderEnd() {
    return renderList_.end();
}

bool Sector::repaintRequired() const {
    return repaintRequired_;
}

void Sector::addDynamicObject(world::IngameObject* obj) {
    renderList_.push_back(obj);
    renderListSortRequired_ = true;

    obj->repaintRectangle();

    obj->onAddedToSector(this);
}

void Sector::removeDynamicObject(world::IngameObject* obj) {
    renderList_.remove(obj);

    obj->repaintRectangle();

    obj->onRemovedFromSector(this);
}

void Sector::requestSort() {
    renderListSortRequired_ = true;
}

boost::shared_ptr<world::IngameObject> Sector::getFirstObjectAt(int worldX, int worldY, bool getTopObject) const {
    boost::shared_ptr<world::IngameObject> ret;

    std::list<world::IngameObject*>::const_reverse_iterator iter = renderList_.rbegin();
    std::list<world::IngameObject*>::const_reverse_iterator end = renderList_.rend();

    for (; iter != end; ++iter) {
        IngameObject* curObj = *iter;
        if (curObj->isVisible() && curObj->hasWorldPixel(worldX, worldY)) {
            if (getTopObject) {
                ret = curObj->getTopParent();
            } else {
                ret = curObj->shared_from_this();
            }
            break;
        }
    }

    return ret;
}

void Sector::getWalkObjectsOn(unsigned int x, unsigned int y, std::list<world::IngameObject*>& list) const {
    world::StaticItem compareDummy;
    compareDummy.getWorldRenderData().setRenderDepth(x, y, -128, 0, 0, 0);
    std::list<world::IngameObject*>::const_iterator iter = std::lower_bound(renderList_.begin(), renderList_.end(), &compareDummy, &Sector::renderDepthSortHelper);

    compareDummy.getWorldRenderData().setRenderDepth(x + 1, y, -128, 0, 0, 0);
    std::list<world::IngameObject*>::const_iterator end = std::upper_bound(renderList_.begin(), renderList_.end(), &compareDummy, &Sector::renderDepthSortHelper);

    for (; iter != end; ++iter) {
        world::IngameObject* curObj = *iter;
        if (curObj->getLocXGame() == x && curObj->getLocYGame() == y) {
            if (curObj->isStaticItem() || curObj->isDynamicItem() || curObj->isMap()) {
                list.push_back(curObj);
            }
        }
    }

    list.sort(&Sector::renderDepthSortHelper);
}

bool Sector::checkMovement(const CL_Vec3f& curLocation, int stepReach, CL_Vec3f& outLoc) const {
    std::list<world::IngameObject*> itemList;
    getWalkObjectsOn(outLoc.x, outLoc.y, itemList);

    std::list<world::IngameObject*>::const_iterator iter = itemList.begin();
    std::list<world::IngameObject*>::const_iterator end = itemList.end();

    outLoc.z = -99999;
    int curLocMaxCheck = ceilf(curLocation.z + 15);
    bool movePossible = false;

    for (; iter != end; ++iter) {
        world::IngameObject* curObj = *iter;

        // first, check if we can step on this tile
        if (curObj->isMap()) {
            world::MapTile* mapTile = static_cast<world::MapTile*>(curObj);
            if (mapTile->getTileDataInfo()->impassable()) {
                continue;
            }

            int newZ = mapTile->getAverageZ();
            int maxCheck = (std::max)(curLocMaxCheck, newZ + 15);

            if (newZ > outLoc.z && checkFreeSpace(itemList, newZ, maxCheck)) {
                movePossible = true;
                outLoc.z = newZ;
            }

        } else {
            // is static or dynamic item, because other stuff was not added to the list
            world::DynamicItem* dynamicObj = nullptr;
            world::StaticItem* staticObj = nullptr;
            bool isDynamic = false;
            if (curObj->isDynamicItem()) {
                isDynamic = true;
                dynamicObj = static_cast<world::DynamicItem*>(curObj);
            } else {
                staticObj = static_cast<world::StaticItem*>(curObj);
            }
            const data::StaticTileInfo* tileInfo = isDynamic ? dynamicObj->getTileDataInfo() : staticObj->getTileDataInfo();

            if (!tileInfo->surface() || tileInfo->impassable()) {
                continue;
            }

            bool isBridge = tileInfo->bridge();
            int newZ = curObj->getLocZGame();
            int checkZ = newZ + tileInfo->height_;
            if (isBridge) {
                // with bridge items, check that we can reach the base of the item
                if (newZ > stepReach) {
                    continue;
                }
                newZ += floorf(tileInfo->height_ / 2.0f);
            } else {
                // for non-bridges, we have to be able to reach the top
                newZ += tileInfo->height_;
                if (newZ > stepReach) {
                    continue;
                }
            }

            int maxCheck = (std::max)(curLocMaxCheck, newZ + 15);

            if (newZ > outLoc.z && checkFreeSpace(itemList, checkZ, maxCheck)) {
                movePossible = true;
                outLoc.z = newZ;
            }
        }
    }

    return movePossible;
}

bool Sector::checkFreeSpace(const std::list<world::IngameObject*>& checkList, int zFrom, int zTo) const {
    std::list<world::IngameObject*>::const_iterator iter = checkList.begin();
    std::list<world::IngameObject*>::const_iterator end = checkList.end();

    for (; iter != end; ++iter) {
        world::IngameObject* curObj = *iter;

        int itemZ = curObj->getLocZGame();
        if (itemZ > zTo) {
            break;
        }

        int itemTop = itemZ;
        bool canPass = true;

        if (curObj->isMap()) {
            canPass = !((world::MapTile*)curObj)->getTileDataInfo()->impassable();
        } else if (curObj->isStaticItem()) {
            world::StaticItem* staticObj = (world::StaticItem*)(curObj);
            const data::StaticTileInfo* tileInfo = staticObj->getTileDataInfo();
            itemTop += tileInfo->bridge() ? ceilf(tileInfo->height_ / 2.0) :  tileInfo->height_;
            canPass = !tileInfo->surface() && !tileInfo->impassable();
        } else if (curObj->isDynamicItem()) {
            world::DynamicItem* dynamicObj = (world::DynamicItem*)(curObj);
            const data::StaticTileInfo* tileInfo = dynamicObj->getTileDataInfo();
            itemTop += tileInfo->bridge() ? ceilf(tileInfo->height_ / 2.0) :  tileInfo->height_;
            canPass = !tileInfo->surface() && !tileInfo->impassable();
        }

        if (zFrom < itemTop && zTo > itemZ && !canPass) {
            return false;
        }
    }

    return true;
}

int Sector::getStepReach(const CL_Vec3f& loc) const {
    int ret = loc.z + 2;
    // if we are currently standing on a bridge or map tile, we can reach (bridge.top + 2), otherwise just (loc.z + 2)
    std::list<world::IngameObject*> itemList;
    getWalkObjectsOn(loc.x, loc.y, itemList);

    std::list<world::IngameObject*>::const_iterator iter = itemList.begin();
    std::list<world::IngameObject*>::const_iterator end = itemList.end();

    for (; iter != end; ++iter) {
        world::IngameObject* curObj = *iter;
        if (curObj->getLocZGame() > loc.z) {
            break;
        }

        if (curObj->isMap()) {
            world::MapTile* mapTile = (world::MapTile*)(curObj);
            int top = mapTile->getMaxZ();
            if (!mapTile->getTileDataInfo()->impassable() && top > ret) {
                ret = top;
            }
        } else if (curObj->isStaticItem()) {
            world::StaticItem* staticObj = (world::StaticItem*)(curObj);
            const data::StaticTileInfo* tileInfo = staticObj->getTileDataInfo();

            int top = curObj->getLocZGame() + tileInfo->height_ + 2;
            if (!tileInfo->impassable() && tileInfo->surface() && tileInfo->bridge() && top > ret) {
                ret = top;
            }
        } else if (curObj->isDynamicItem()) {
            world::DynamicItem* dynamicObj = (world::DynamicItem*)(curObj);
            const data::StaticTileInfo* tileInfo = dynamicObj->getTileDataInfo();
            int top = curObj->getLocZGame() + tileInfo->height_ + 2;
            if (!tileInfo->impassable() && tileInfo->surface() && tileInfo->bridge() && top > ret) {
                ret = top;
            }
        }
    }

    return ret;
}

boost::shared_ptr<MapTile> Sector::getMapTileAt(unsigned int worldX, unsigned int worldY) const {
    unsigned int myX = worldX % 8;
    unsigned int myY = worldY % 8;

    return mapBlock_->get(myX, myY);
}

void Sector::invalidateAllTextures() {
    if (mapBlock_ && mapBlock_->isReadComplete()) {
        for (unsigned int x = 0; x < 8; ++x) {
            for (unsigned int y = 0; y < 8; ++y) {
                mapBlock_->get(x, y)->invalidateTextureProvider();
            }
        }
    }

    if (staticBlock_ && staticBlock_->isReadComplete()) {
        std::list<boost::shared_ptr<world::StaticItem> >::iterator it = staticBlock_->getItemList().begin();
        std::list<boost::shared_ptr<world::StaticItem> >::iterator end = staticBlock_->getItemList().end();

        for (; it != end; ++it) {
            (*it)->invalidateTextureProvider();
        }
    }

    fullUpdateRenderDataRequired_ = true;
}

}
}
