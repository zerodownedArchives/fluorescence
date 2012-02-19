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


#ifndef FLUO_UI_CLIPRECTMANAGER_HPP
#define FLUO_UI_CLIPRECTMANAGER_HPP

#include <vector>
#include <ClanLib/Core/Math/rect.h>

namespace fluo {
namespace world {
class IngameObject;
}

namespace ui {

class ClipRectManager {
public:
    ClipRectManager();
    
    void add(const CL_Rectf& rect);
    void clear();
    size_t size() const;
    
    bool isInside(const world::IngameObject* obj) const;
    
    std::vector<CL_Rectf>::const_iterator begin() const;
    std::vector<CL_Rectf>::const_iterator end() const;
    
    void clamp(const CL_Vec2f& topleft, const CL_Size& size);
    
private:
    std::vector<CL_Rectf> rectangles_;
};

}
}

#endif

