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

            if (staticsAddedToList_) {
                std::list<boost::shared_ptr<world::StaticItem> >::iterator it = staticBlock_->getItemList().begin();
                std::list<boost::shared_ptr<world::StaticItem> >::iterator end = staticBlock_->getItemList().end();

                for (; it != end; ++it) {
                    if ((*it)->periodicRenderUpdateRequired()) {
                        quickRenderUpdateList_.push_back(it->get());
                    }
                }
            }

            fullUpdateRenderDataRequired_ = false;

            //LOG_DEBUG << "Items in quicklist: " << quickRenderUpdateList_.size() << std::endl;
            
            
            // add repaint rectangle over full sector range, and a bit more
            const CL_Vec3f* vertCoords = mapBlock_->get(0, 0)->getWorldRenderData().getVertexCoordinates();
            float updateLeft = vertCoords[0].x - 150;
            float updateTop = vertCoords[0].y - 150;
            vertCoords = mapBlock_->get(7, 7)->getWorldRenderData().getVertexCoordinates();
            float updateRight = vertCoords[5].x + 100;
            float updateBottom = vertCoords[5].y + 100;
            CL_Rectf updateRect(updateLeft, updateTop, updateRight, updateBottom);
            ui::Manager::getClipRectManager()->add(updateRect);
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
        if (curObj->isVisible() && curObj->hasPixel(worldX, worldY)) {
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

bool Sector::checkMovement(const CL_Vec3f& curLocation, CL_Vec3f& outLoc) const {
    world::StaticItem compareDummy;
    compareDummy.getWorldRenderData().setRenderDepth(outLoc.x, outLoc.y, -128, 0, 0, 0);
    std::list<world::IngameObject*>::const_iterator iter = std::lower_bound(renderList_.begin(), renderList_.end(), &compareDummy, &Sector::renderDepthSortHelper);
    
    compareDummy.getWorldRenderData().setRenderDepth(outLoc.x + 1, outLoc.y, -128, 0, 0, 0);
    std::list<world::IngameObject*>::const_iterator end = std::upper_bound(renderList_.begin(), renderList_.end(), &compareDummy, &Sector::renderDepthSortHelper);
    
    
    std::list<world::IngameObject*> itemList;
    for (; iter != end; ++iter) {
        world::IngameObject* curObj = *iter;
        if (curObj->getLocXGame() == outLoc.x && curObj->getLocYGame() == outLoc.y) {
            if (curObj->isStaticItem() || curObj->isDynamicItem() || curObj->isMap()) {
                itemList.push_back(curObj);
            }
        }
    }
    
    itemList.sort(&Sector::renderDepthSortHelper);
    iter = itemList.begin();
    end = itemList.end();
    
    //float curLocMaxCheck = curLocation.z + 15;
    bool movePossible = false;
    
    for (; iter != end; ++iter) {
        world::IngameObject* curObj = *iter;
        
        // first, check if we can step on this tile
        if (curObj->isMap()) {
            world::MapTile* mapTile = dynamic_cast<world::MapTile*>(curObj);
            if (mapTile->getTileDataInfo()->impassable()) {
                continue;
            }
            
            float newZ = mapTile->getAverageZ();
            //float maxCheck = (std::max)(curLocMaxCheck, newZ + 15);
            
            // check
            
            movePossible = true;
            outLoc.z = newZ;
            
        } else {
            // is static or dynamic item, because other stuff was not added to the list
            world::DynamicItem* dynamicObj = dynamic_cast<world::DynamicItem*>(curObj);
            world::StaticItem* staticObj = dynamic_cast<world::StaticItem*>(curObj);
            bool isDynamic = (dynamicObj != nullptr);
            const data::StaticTileInfo* tileInfo = isDynamic ? dynamicObj->getTileDataInfo() : staticObj->getTileDataInfo();
            
            if (!tileInfo->surface() || tileInfo->impassable()) {
                continue;
            }
            
            movePossible = true;
            if (tileInfo->bridge()) {
                outLoc.z = curObj->getLocZGame() + tileInfo->height_ / 2.0f;
            } else {
                outLoc.z = curObj->getLocZGame() + tileInfo->height_;
            }
        }
    }
    
    outLoc.z = floor(outLoc.z);
    return movePossible;
}

}
}
