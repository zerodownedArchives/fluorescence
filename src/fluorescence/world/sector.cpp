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

#include <data/manager.hpp>
#include <data/staticsloader.hpp>
#include <data/maploader.hpp>

#include <ui/manager.hpp>
#include <ui/render/renderqueue.hpp>

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

void Sector::removeFromRenderQueue(boost::shared_ptr<ui::RenderQueue> rq) {
    if (staticBlock_) {
        std::list<boost::shared_ptr<world::StaticItem> > staticList = staticBlock_->getItemList();
        std::list<boost::shared_ptr<world::StaticItem> >::const_iterator it = staticList.begin();
        std::list<boost::shared_ptr<world::StaticItem> >::const_iterator end = staticList.end();

        for (; it != end; ++it) {
            (*it)->removeFromRenderQueue(rq);
        }
    }

    if (mapBlock_) {
        for (unsigned int x = 0; x < 8; ++x) {
            for (unsigned int y = 0; y < 8; ++y) {
                mapBlock_->get(x, y)->removeFromRenderQueue(rq);
            }
        }
    }
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
                repaintRequired_ |= mapBlock_->get(x, y)->textureOrVerticesChanged();
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

            std::list<boost::shared_ptr<world::StaticItem> >::iterator it = staticBlock_->getItemList().begin();
            std::list<boost::shared_ptr<world::StaticItem> >::iterator end = staticBlock_->getItemList().end();

            for (; it != end; ++it) {
                if ((*it)->periodicRenderUpdateRequired()) {
                    quickRenderUpdateList_.push_back(it->get());
                }
            }

            fullUpdateRenderDataRequired_ = false;

            //LOG_DEBUG << "Items in quicklist: " << quickRenderUpdateList_.size() << std::endl;
        }
    } else if (!quickRenderUpdateList_.empty()) {
        //LOG_DEBUG << "quick render update, size=" << quickRenderUpdateList_.size() << std::endl;
        // only call update on those few things that really need to be updated (e.g. animated statics)
        std::list<world::IngameObject*>::iterator iter = quickRenderUpdateList_.begin();
        std::list<world::IngameObject*>::iterator end = quickRenderUpdateList_.end();

        for (; iter != end; ++iter) {
            (*iter)->updateRenderData(elapsedMillis);
            renderListSortRequired_ |= (*iter)->renderDepthChanged();
            repaintRequired_ |= (*iter)->textureOrVerticesChanged();
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

}
}
