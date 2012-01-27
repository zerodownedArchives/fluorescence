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


#ifndef FLUO_DATA_GUMPARTLOADER_HPP
#define FLUO_DATA_GUMPARTLOADER_HPP

#include "weakptrcache.hpp"
#include "indexedondemandfileloader.hpp"

#include <boost/filesystem.hpp>

namespace fluo {

namespace ui {
    class Texture;
}

namespace data {

class GumpArtLoader {
public:
    GumpArtLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath);

    boost::shared_ptr<ui::Texture> getTexture(unsigned int id);

    void readCallback(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<ui::Texture>, unsigned int extra, unsigned int userData);

    bool hasTexture(unsigned int id);

private:
    WeakPtrCache<ui::Texture, IndexedOnDemandFileLoader> cache_;
};

}
}

#endif
