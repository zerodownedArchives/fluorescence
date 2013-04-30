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



#include "radarcolloader.hpp"

#include <boost/bind.hpp>

#include "fullfileloader.hpp"
#include "util.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace data {

RadarColLoader::RadarColLoader(const boost::filesystem::path& path) : colorCount_(0), colors_(nullptr) {
    FullFileLoader loader(path);
    loader.read(boost::bind(&RadarColLoader::readCallback, this, _1, _2));
}

RadarColLoader::~RadarColLoader() {
    if (colors_) {
        delete [] colors_;
        colors_ = nullptr;
    }
}

void RadarColLoader::readCallback(int8_t* buf, unsigned int len) {
    colorCount_ = len / 2;
    colors_ = new uint32_t[colorCount_];

    uint16_t* ptr = reinterpret_cast<uint16_t*>(buf);

    for (unsigned int i = 0; i < colorCount_; ++i) {
        colors_[i] = Util::getColorRGBA(*ptr);
        ++ptr;
    }

    LOG_DEBUG << "Total read bytes: " << (reinterpret_cast<int8_t*>(ptr) - buf) << " len: " << len << std::endl;
}

uint32_t RadarColLoader::getMapColor(unsigned int id) const {
    return id < colorCount_ ? colors_[id] : 0;
}

uint32_t RadarColLoader::getStaticColor(unsigned int id) const {
    id += 0x4000;
    return id < colorCount_ ? colors_[id] : 0;
}

}
}
