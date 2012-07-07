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

#include "patcherupdater.hpp"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <misc/log.hpp>

namespace fluo {

void PatcherUpdater::copyPatcherUpdateFolder() {
    namespace bfs = boost::filesystem;

    LOG_INFO << "Updating patcher files..." << std::endl;
    unsigned int counter = 0;

    try {
        bfs::directory_iterator nameIter("patcher_update");
        bfs::directory_iterator nameEnd;

        for (; nameIter != nameEnd; ++nameIter) {
            if (bfs::is_regular_file(nameIter->status()) && nameIter->leaf() != ".svn") {
                if (bfs::exists(nameIter->path().leaf())) {
                    bfs::remove(nameIter->path().leaf());
                }
                bfs::copy_file(nameIter->path(), nameIter->path().leaf());
                LOG_DEBUG << "Copying patcher update file " << nameIter->path().leaf() << std::endl;
                ++counter;
            }
        }
    } catch (bfs::basic_filesystem_error<bfs::path>& err) {
        LOG_ERROR << "Unable to update patcher files: " << err.what() << std::endl;
    }

    LOG_INFO << counter << " patcher update files copied" << std::endl;
}

}