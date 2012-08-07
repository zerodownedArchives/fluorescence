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

#include "filepathloader.hpp"

#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <ClanLib/Core/System/exception.h>
#include <ClanLib/Display/Image/pixel_buffer.h>

#include <ui/texture.hpp>
#include <misc/log.hpp>

#include "manager.hpp"

namespace fluo {
namespace data {

FilePathLoader::FilePathLoader() {
    boost::shared_ptr<OnDemandUrlLoader<boost::filesystem::path, ui::Texture> > threadedLoader(new OnDemandUrlLoader<boost::filesystem::path, ui::Texture>(boost::bind(&FilePathLoader::readTextureCallback, this, _1, _2)));
    cache_.init(threadedLoader);
}

boost::shared_ptr<ui::Texture> FilePathLoader::getTexture(const boost::filesystem::path& path) {
    return cache_.get(path);
}

void FilePathLoader::readTextureCallback(const boost::filesystem::path& path, boost::shared_ptr<ui::Texture> tex) {
    if (!boost::filesystem::exists(path) || !boost::filesystem::is_regular_file(path)) {
        LOG_ERROR << "Image file not found: " << path << std::endl;
        return;
    }

    //LOG_DEBUG << "Opening image from file " << path << std::endl;

    try {
        CL_PixelBuffer pxBuf(path.string());
        if (pxBuf.get_format() != cl_rgba8) {
            // LOG_DEBUG << "reformat required!" << std::endl;
            pxBuf = pxBuf.to_format(cl_rgba8);
        }
        tex->setTexture(pxBuf);
    } catch (CL_Exception& ex) {
        LOG_ERROR << "Provided file can not be interpreted as a picture: " << path.string() << std::endl;
    }
}

}
}

