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



#include "worldrenderqueue.hpp"

#include <boost/bind.hpp>

#include <misc/log.hpp>

#include <world/ingameobject.hpp>
#include <world/sector.hpp>

#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

namespace fluo {
namespace ui {

bool WorldRenderQueue::batchedComparator(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b) {
    unsigned long texA = (unsigned long)a->getIngameTexture().get();
    unsigned long texB = (unsigned long)b->getIngameTexture().get();

    if (texA == texB) {
        return (unsigned long)a.get() <= (unsigned long)b.get();
    } else {
        return texA <= texB;
    }
}

WorldRenderQueue::WorldRenderQueue() : RenderQueue(boost::bind(&WorldRenderQueue::batchedComparator, this, _1, _2)) {
}

WorldRenderQueue::~WorldRenderQueue() {
}


void WorldRenderQueue::preRender() {
    if (!removeList_.empty()) {
        sort();
        processRemoveList();
        forceRepaint_ = true;
    }

    if (!addList_.empty()) {
        updateMinMaxRenderDepth();
    }

    bool objectsAdded = processAddList();

    if (objectsAdded) {
        //sort();
    }
}

void WorldRenderQueue::postRender(bool renderingComplete) {
    resetWorldRepaintIndicators();

    // force repaint if the rendering was not complete (e.g. a texture not yet loaded)
    forceRepaint_ = !renderingComplete;
}

boost::shared_ptr<world::IngameObject> WorldRenderQueue::getFirstObjectAt(int worldX, int worldY, bool getTopParent) {
    RenderQueue::iterator igIter = begin();
    RenderQueue::iterator igEnd = end();

    boost::shared_ptr<world::IngameObject> ret;

    RenderDepth maxRenderPrio = 0;

    for (; igIter != igEnd; ++igIter) {
        boost::shared_ptr<world::IngameObject> curObj = *igIter;
        if (curObj->isVisible() && curObj->hasPixel(worldX, worldY) && curObj->getRenderDepth() > maxRenderPrio) {
            if (getTopParent) {
                ret = curObj->getTopParent();
            } else {
                ret = curObj;
            }
            maxRenderPrio = curObj->getRenderDepth();
        }
    }

    return ret;
}

}
}
