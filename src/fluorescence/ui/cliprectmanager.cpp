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

#include "cliprectmanager.hpp"

#include <misc/log.hpp>
#include <world/ingameobject.hpp>

namespace fluo {
namespace ui {
    
ClipRectManager::ClipRectManager() {
}
    
void ClipRectManager::add(const CL_Rectf& rect) {
    boost::mutex::scoped_lock myLock(mutex_);
    
    // happens when the location is set for the first time (invalid previous coordinates)
    if (rect.top == rect.bottom || rect.left == rect.right) {
        return;
    }
    
    CL_Rectf expanded(rect);
    expanded.expand(1, 1, 1, 1);

    // check for overlaps to keep number of rectangles as small as possible
    std::vector<CL_Rectf>::iterator iter = rectangles_.begin();
    std::vector<CL_Rectf>::iterator end = rectangles_.end();
    
    for (; iter != end; ++iter) {
        if (iter->is_overlapped(expanded)) {
            CL_Rectf bounding = CL_Rectf(*iter).bounding_rect(expanded);
            CL_Rectf overlap = CL_Rectf(*iter).overlap(expanded);
            
            // area of the bounding rectangle should not be too big. keep the rects seperate if joining them does not give a significant advantage
            if ((bounding.get_width() * bounding.get_height() / 2) <= (iter->get_width() * iter->get_height() + expanded.get_width() * expanded.get_height() - overlap.get_width() * overlap.get_height())) {
                iter->bounding_rect(expanded);
                return;
            }
        }
    }
    
    rectangles_.push_back(expanded);
}

void ClipRectManager::clear() {
    rectangles_.clear();
}

std::vector<CL_Rectf>::const_iterator ClipRectManager::begin() const {
    return rectangles_.begin();
}

std::vector<CL_Rectf>::const_iterator ClipRectManager::end() const {
    return rectangles_.end();
}

size_t ClipRectManager::size() const {
    return rectangles_.size();
}

void ClipRectManager::clamp(const CL_Vec2f& topleftBase, const CL_Size& sizeBase) {
    if (rectangles_.empty()) {
        return;
    }
    
    std::vector<CL_Rectf> clamped;
    
    CL_Rectf clampRect(topleftBase.x, topleftBase.y, CL_Sizef(sizeBase.width, sizeBase.height));
    
    std::vector<CL_Rectf>::iterator iter = rectangles_.begin();
    std::vector<CL_Rectf>::iterator end = rectangles_.end();
    std::vector<CL_Rectf>::iterator helper;
    
    for (; iter != end; ++iter) {
        if (clampRect.is_overlapped(*iter)) {
            clamped.push_back(iter->clip(clampRect));
        }
    }
    
    rectangles_ = clamped;
}

bool ClipRectManager::overlapsAny(world::IngameObject* obj) {
    std::vector<CL_Rectf>::iterator iter = rectangles_.begin();
    std::vector<CL_Rectf>::iterator end = rectangles_.end();
    
    for (; iter != end; ++iter) {
        if (obj->overlaps(*iter)) {
            return true;
        }
    }
    
    return false;
}
    
}
}

