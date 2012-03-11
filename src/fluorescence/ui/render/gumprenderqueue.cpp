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



#include "gumprenderqueue.hpp"

#include <boost/bind.hpp>

#include <client.hpp>
#include <misc/log.hpp>

#include <world/ingameobject.hpp>
#include <world/sector.hpp>

#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

namespace fluo {
namespace ui {

bool GumpRenderQueue::renderDepthComparator(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b) {
    // mobile is always rendered first;
    if (a->isMobile()) {
        return true;
    } else if (b->isMobile()) {
        return false;
    }

    if (!a->isDynamicItem() || !b->isDynamicItem()) {
        // why is that in this queue?
        LOG_WARN << "Object other than mobile or item in GumpRenderQueue" << std::endl;
        return true;
    }

    boost::shared_ptr<world::DynamicItem> aDyn = boost::dynamic_pointer_cast<world::DynamicItem>(a);
    boost::shared_ptr<world::DynamicItem> bDyn = boost::dynamic_pointer_cast<world::DynamicItem>(b);

    unsigned int aLayer = aDyn->getLayer() - 1;
    unsigned int bLayer = bDyn->getLayer() - 1;

    if (aLayer >= layerPriorities_.size()) {
        // LOG_WARN << "Rendering item with invalid layer " << aDyn->getLayer() << ". Unable to assign render priority" << std::endl;
        aLayer = 0;
    }

    if (bLayer >= layerPriorities_.size()) {
        // LOG_WARN << "Rendering item with invalid layer " << bDyn->getLayer() << ". Unable to assign render priority" << std::endl;
        bLayer = 0;
    }

    if (layerPriorities_[aLayer] == layerPriorities_[bLayer]) {
        LOG_WARN << "2 Items at same layer in GumpRenderQueue::renderDepthComparator" << std::endl;
        return true;
    }

    return layerPriorities_[aLayer] <= layerPriorities_[bLayer];
}

GumpRenderQueue::GumpRenderQueue() : RenderQueue(boost::bind(&GumpRenderQueue::renderDepthComparator, this, _1, _2)) {
    layerPriorities_ = Client::getSingleton()->getConfig()["/fluo/ui/layer-priorities@paperdoll"].asIntList();
}

GumpRenderQueue::~GumpRenderQueue() {
}


void GumpRenderQueue::preRender() {
    if (!removeList_.empty()) {
        processRemoveList();
        forceRepaint_ = true;
    }

    bool requireSort = processAddList();

    if (requireSort || gumpChanged_) {
        sort();
    }
}

void GumpRenderQueue::postRender(bool renderingComplete) {
    resetGumpRepaintIndicators();

    // force repaint if the rendering was not complete (e.g. a texture not yet loaded)
    forceRepaint_ = !renderingComplete;
}

boost::shared_ptr<world::IngameObject> GumpRenderQueue::getFirstObjectAt(int pixelX, int pixelY, bool getTopParent) {
    RenderQueue::reverse_iterator igIter = rbegin();
    RenderQueue::reverse_iterator igEnd = rend();

    boost::shared_ptr<world::IngameObject> ret;

    for (; igIter != igEnd; ++igIter) {
        boost::shared_ptr<world::IngameObject> curObj = *igIter;
        if (curObj->isDynamicItem() && curObj->isVisible() && curObj->hasGumpPixel(pixelX, pixelY)) {

            if (getTopParent) {
                curObj = curObj->getTopParent();
            }

            ret = curObj;
            break;
        }
    }

    return ret;
}

}
}
