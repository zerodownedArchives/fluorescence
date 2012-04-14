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
 
#include "httploader.hpp"

#include <boost/bind.hpp>
#include <ClanLib/Core/System/exception.h>
#include <ClanLib/Display/Image/pixel_buffer.h>
#include <ClanLib/Display/ImageProviders/provider_factory.h>

#include <ui/texture.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace data {
 
HttpLoader::HttpLoader() : dataBuffer_(1024 * 100), memoryIO_(dataBuffer_) {
    handle_ = curl_easy_init();
    curl_easy_setopt(handle_, CURLOPT_WRITEFUNCTION, HttpLoader::curlRecvCallback);
    
    boost::shared_ptr<OnDemandUrlLoader<UnicodeString, ui::Texture> > threadedLoader(new OnDemandUrlLoader<UnicodeString, ui::Texture>(boost::bind(&HttpLoader::readTextureCallback, this, _1, _2)));
    cache_.init(threadedLoader);
}

HttpLoader::~HttpLoader() {
    curl_easy_cleanup(handle_);
}

boost::shared_ptr<ui::Texture> HttpLoader::getTexture(const UnicodeString& url) {
    return cache_.get(url);
}

void HttpLoader::readTextureCallback(const UnicodeString& url, boost::shared_ptr<ui::Texture> tex) {
    LOG_DEBUG << "Opening image from url " << StringConverter::toUtf8String(url).c_str() << std::endl;
    
    tex->setUsage(ui::Texture::USAGE_GUMP);
    
    curl_easy_setopt(handle_,CURLOPT_WRITEDATA, this);
    
    curl_easy_setopt(handle_, CURLOPT_URL, StringConverter::toUtf8String(url).c_str());
    memoryIO_.seek(0, CL_IODevice::seek_set);
    
    CURLcode err = curl_easy_perform(handle_);
    if (err) {
        LOG_ERROR << "CURL error: " << curl_easy_strerror(err) << std::endl;
    }
    
    memoryIO_.seek(0, CL_IODevice::seek_set);
    try {
        UnicodeString extension(url, url.lastIndexOf('.') + 1);
        LOG_DEBUG << "extension: " << extension << std::endl;
        CL_PixelBuffer pxBuf = CL_ImageProviderFactory::load(memoryIO_, StringConverter::toUtf8String(extension));
        if (pxBuf.get_format() != cl_rgba8) {
            // LOG_DEBUG << "reformat required!" << std::endl;
            pxBuf = pxBuf.to_format(cl_rgba8);
        }
        tex->setTexture(pxBuf);
    } catch (CL_Exception& ex) {
        LOG_ERROR << "Provided URL can not be interpreted as a picture: " << url << std::endl;
    }
}

size_t HttpLoader::curlRecvCallback(void* ptr, size_t size, size_t nmemb, void* userdata) {
    HttpLoader* self = (HttpLoader*)userdata;
    return self->memoryIO_.write(ptr, size * nmemb, true);
}

}
}

