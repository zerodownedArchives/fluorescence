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
    
ClipRectManager::ClipRectManager() : forceRepaint_(false) {
}
    
void ClipRectManager::add(const CL_Rectf& rect) {
    // happens when the location is set for the first time (invalid previous coordinates)
    if (rect.top == rect.bottom || rect.left == rect.right) {
        return;
    }
    
    CL_Rectf expanded(rect);
    expanded.expand(1, 1, 1, 1);
    
    boost::recursive_mutex::scoped_lock(mutex_);
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
    if (forceRepaint_) {
        CL_Rectf rect(topleftBase.x, topleftBase.y, CL_Sizef(sizeBase.width, sizeBase.height));
        rectangles_.clear();
        rectangles_.push_back(rect);
    } else if (rectangles_.empty()) {
        return;
    }
    
    std::vector<CL_Rectf> clamped;
    
    CL_Rectf clampRect(topleftBase.x, topleftBase.y, CL_Sizef(sizeBase.width, sizeBase.height));
    
    std::vector<CL_Rectf>::iterator iter = rectangles_.begin();
    std::vector<CL_Rectf>::iterator end = rectangles_.end();
    
    for (; iter != end; ++iter) {
        if (clampRect.is_overlapped(*iter)) {
            
            bool addedWithExpand = false;
            CL_Rectf curClamped = iter->clip(clampRect);
            
            float curClampedArea = curClamped.get_width() * curClamped.get_height();
            
            // check for overlaps to keep number of rectangles as small as possible
            std::vector<CL_Rectf>::iterator mergeIter = clamped.begin();
            std::vector<CL_Rectf>::iterator mergeEnd = clamped.end();
            
            for (; mergeIter != mergeEnd; ++mergeIter) {
                if (mergeIter->is_overlapped(curClamped)) {
                    CL_Rectf bounding = CL_Rectf(*mergeIter).bounding_rect(curClamped);
                    CL_Rectf overlap = CL_Rectf(*mergeIter).overlap(curClamped);
                    
                    // area of the bounding rectangle should not be too big. keep the rects seperate if joining them does not give a significant advantage
                    float unnecessaryIncluded = 
                            (bounding.get_width() * bounding.get_height())
                            - (mergeIter->get_width() * mergeIter->get_height() + curClampedArea)
                            + (overlap.get_width() * overlap.get_height());
                            
                    if (unnecessaryIncluded <= curClampedArea * 3) {
                                
                        mergeIter->bounding_rect(curClamped);
                        addedWithExpand = true;
                        break;
                    }
                }
            }
            
            if (!addedWithExpand) {
                clamped.push_back(curClamped);
            }
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

void ClipRectManager::forceFullRepaint() {
    forceRepaint_ = true;
}
    
}
}

