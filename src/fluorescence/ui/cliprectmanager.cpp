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

#include <world/ingameobject.hpp>

namespace fluo {
namespace ui {
    
ClipRectManager::ClipRectManager() {
}
    
void ClipRectManager::add(const CL_Rectf& rect) {
    // TODO: check for overlaps to keep number of rectangles as small as possible
    rectangles_.push_back(rect);
}

void ClipRectManager::clear() {
    rectangles_.clear();
}

bool ClipRectManager::isInside(const world::IngameObject* obj) const {
    if (rectangles_.empty()) {
        return false;
    }
    
    std::vector<CL_Rectf>::const_iterator iter = rectangles_.begin();
    std::vector<CL_Rectf>::const_iterator end = rectangles_.end();
    
    for (; iter != end; ++iter) {
        if (obj->overlaps(*iter)) {
            return true;
        }
    }
    
    return false;
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
    
}
}

