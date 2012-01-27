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



#include "fullfileloader.hpp"

namespace fluo {
namespace data {

FullFileLoader::FullFileLoader(const boost::filesystem::path& path) : path_(path) {
    if (!boost::filesystem::exists(path) || !boost::filesystem::is_regular_file(path)) {
        throw Exception("File not found");
    }
}

void FullFileLoader::read(ReadCallback readCallback) {
    boost::filesystem::ifstream stream(path_, std::ios_base::binary);
    if (stream.is_open()) {
        // calc how many bytes we want to read
        unsigned int filesize = boost::filesystem::file_size(path_);

        int8_t* buf = reinterpret_cast<int8_t*>(malloc(filesize));

        stream.seekg(0, std::ios_base::beg);

        stream.read(reinterpret_cast<char*>(buf), filesize);
        stream.close();

        readCallback(buf, filesize);

        free(buf);
    } else {
        throw Exception("Error opening stream");
    }
}

}
}
