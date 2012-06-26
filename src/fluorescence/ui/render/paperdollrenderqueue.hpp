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


#ifndef FLUO_UI_PAPERDOLLRENDERQUEUE_HPP
#define FLUO_UI_PAPERDOLLRENDERQUEUE_HPP

#include <list>
#include <boost/shared_ptr.hpp>

#include <ui/render/renderqueue.hpp>

namespace fluo {

namespace world {
    class IngameObject;
    class Sector;
}

namespace ui {

class PaperdollRenderQueue : public RenderQueue {
public:
    PaperdollRenderQueue();
    ~PaperdollRenderQueue();

    virtual void preRender();
    virtual void postRender(bool renderingComplete);

    virtual boost::shared_ptr<world::IngameObject> getFirstObjectAt(int worldX, int worldY, bool getTopParent);

private:
    /// used to sort the objects according to their layer
    bool renderDepthComparator(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b);

    std::vector<int> layerPriorities_;
};

}
}

#endif
