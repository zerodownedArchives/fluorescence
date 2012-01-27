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


#ifndef FLUO_DATA_ANIMDATALOADER_HPP
#define FLUO_DATA_ANIMDATALOADER_HPP

#include <list>
#include <map>

#include <boost/filesystem/path.hpp>

namespace fluo {
namespace data {

struct AnimDataInfo {
    AnimDataInfo() : frameCount_(0) {
    }

    unsigned int artIds_[64];
    unsigned int frameCount_;
    unsigned int frameIntervalMillis_;
    unsigned int frameStart_;
};

class AnimDataLoader {
public:
    AnimDataLoader(const boost::filesystem::path& path);

    void readCallback(int8_t* buf, unsigned int len);

    AnimDataInfo getInfo(unsigned int artId);

private:
    std::map<unsigned int, AnimDataInfo> infos_;

};

}
}

#endif
