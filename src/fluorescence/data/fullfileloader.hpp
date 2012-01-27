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


#ifndef FLUO_DATA_FULLFILELOADER_HPP
#define FLUO_DATA_FULLFILELOADER_HPP

#include <misc/exception.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/function.hpp>

namespace fluo {
namespace data {

/**
 * \brief This class is used to load a full file into memory
 */
class FullFileLoader {
public:

    typedef boost::function<void (int8_t*, unsigned int)> ReadCallback;

    /**
     * \throw DataException when file is nonexistant or directory
     */
    FullFileLoader(const boost::filesystem::path& path);

    /// brief Read full file to memory and call callback
    void read(ReadCallback readCallback);

private:
    boost::filesystem::path path_;
};

}
}

#endif
