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


#ifndef FLUO_DATA_ANIMLOADER_HPP
#define FLUO_DATA_ANIMLOADER_HPP

#include "weakptrcache.hpp"
#include "indexedondemandfileloader.hpp"

#include <boost/filesystem.hpp>

namespace fluo {

namespace ui {
    class Animation;
}

namespace data {

class AnimLoader {
public:
    AnimLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath, unsigned int highDetailCount, unsigned int lowDetailCount);

    boost::shared_ptr<ui::Animation> getAnimation(unsigned int bodyId, unsigned int animId, unsigned int direction);

    void readCallback(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<ui::Animation> anim, unsigned int extra, unsigned int userData);

private:
    unsigned int highDetailCount_;
    unsigned int lowDetailCount_;
    WeakPtrCache<ui::Animation, IndexedOnDemandFileLoader> cache_;
};

}
}

#endif
