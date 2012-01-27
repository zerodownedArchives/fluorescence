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


#ifndef FLUO_DATA_CLILOCLOADER_HPP
#define FLUO_DATA_CLILOCLOADER_HPP

#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

#include <misc/string.hpp>

namespace fluo {
namespace data {

struct ClilocEntry {
    ClilocEntry() : id_(0), len_(0), streampos_(0), useCount_(0), fromEnu_(true) {
    }

    unsigned int id_;
    unsigned int len_;
    unsigned int streampos_;
    unsigned int useCount_; // if the string is accessed often, cache it
    UnicodeString cachedString_;
    bool fromEnu_;
};

class ClilocLoader {
public:
    ClilocLoader();
    ~ClilocLoader();

    void indexFile(const boost::filesystem::path& path, bool isEnu);

    UnicodeString get(unsigned int id);
    UnicodeString get(unsigned int id, const UnicodeString& paramString);

private:
    std::map<unsigned int, ClilocEntry> entryMap_;

    static const unsigned int USE_COUNT_CACHE_LIMIT = 3;

    UnicodeString readEntry(const ClilocEntry& entry);

    boost::filesystem::ifstream languageStream_;
    boost::filesystem::ifstream enuStream_;

    unsigned int bufferSize_;
    int8_t* buffer_;
};

}
}

#endif
