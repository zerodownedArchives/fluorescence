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


#ifndef FLUO_DATA_HTTP_LOADER_HPP
#define FLUO_DATA_HTTP_LOADER_HPP

#include <curl/curl.h>
#include <boost/shared_ptr.hpp>
#include <ClanLib/Core/IOData/iodevice_memory.h>
#include <ClanLib/Core/System/databuffer.h>

#include <misc/string.hpp>

#include "ondemandurlloader.hpp"
#include "weakptrcache.hpp"

namespace fluo {
namespace ui { 
    class Texture;
}

namespace data {

class HttpLoader {
public:
    HttpLoader();
    ~HttpLoader();
    
    boost::shared_ptr<ui::Texture> getTexture(const UnicodeString& url);

    void readTextureCallback(const UnicodeString& url, boost::shared_ptr<ui::Texture> tex);
    
private:
    CURL* handle_;
    
    CL_DataBuffer dataBuffer_;
    CL_IODevice_Memory memoryIO_;
    
    static size_t curlRecvCallback(void* ptr, size_t size, size_t nmemb, void* stream);
    
    WeakPtrCache<UnicodeString, ui::Texture, OnDemandUrlLoader> cache_;
};    

}
}

#endif

