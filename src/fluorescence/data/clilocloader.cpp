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



#include "clilocloader.hpp"

#include <unicode/regex.h>

#include <net/packetreader.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace data {

ClilocLoader::ClilocLoader() : bufferSize_(0), buffer_(NULL) {
}

ClilocLoader::~ClilocLoader() {
    if (buffer_) {
        free(buffer_);
        buffer_ = NULL;
    }
}

bool ClilocLoader::hasEntry(unsigned int id) const {
    return entryMap_.find(id) != entryMap_.end();
}

UnicodeString ClilocLoader::get(unsigned int id) {
    std::map<unsigned int, ClilocEntry>::iterator iter = entryMap_.find(id);
    if (iter != entryMap_.end()) {
        if (iter->second.useCount_ == USE_COUNT_CACHE_LIMIT) {
            // cache it now
            iter->second.useCount_ = USE_COUNT_CACHE_LIMIT + 1;
            iter->second.cachedString_ = readEntry(iter->second);
            return iter->second.cachedString_;
        } else if (iter->second.useCount_ > USE_COUNT_CACHE_LIMIT) {
            // already cached
            return iter->second.cachedString_;
        } else {
            ++iter->second.useCount_;
            return readEntry(iter->second);
        }
    }

    return UnicodeString("##CLILOCERROR");
}

UnicodeString ClilocLoader::get(unsigned int id, const UnicodeString& paramString) {
    static UnicodeString params[9];
    static UnicodeString paramRegex[9] = {
        "~1_[^~]+~", "~2_[^~]+~", "~3_[^~]+~", "~4_[^~]+~", "~5_[^~]+~",
        "~6_[^~]+~", "~7_[^~]+~", "~8_[^~]+~", "~9_[^~]+~",
    };

    UnicodeString str = get(id);

    if (paramString.length() > 0) {
        UErrorCode status = U_ZERO_ERROR;
        RegexMatcher paramSplitter("\\t", 0, status);
        unsigned int paramCount = paramSplitter.split(paramString, params, 9, status);

        for (unsigned int i = 0; i < paramCount; ++i) {
            RegexMatcher curParamMatcher(paramRegex[i], str, 0, status);

            str = curParamMatcher.replaceAll(params[i], status);
        }
    }

    return str;
}

UnicodeString ClilocLoader::get(unsigned int id, const std::vector<UnicodeString>& params) {
    static UnicodeString paramRegex[9] = {
        "~1_[^~]+~", "~2_[^~]+~", "~3_[^~]+~", "~4_[^~]+~", "~5_[^~]+~",
        "~6_[^~]+~", "~7_[^~]+~", "~8_[^~]+~", "~9_[^~]+~",
    };

    UnicodeString str = get(id);

    if (params.size() > 0) {
        UErrorCode status = U_ZERO_ERROR;

        for (unsigned int i = 0; i < params.size(); ++i) {
            RegexMatcher curParamMatcher(paramRegex[i], str, 0, status);

            str = curParamMatcher.replaceAll(params[i], status);
        }
    }

    return str;
}

void ClilocLoader::indexFile(const boost::filesystem::path& path, bool isEnu) {
    unsigned int startBufferSize = bufferSize_;

    boost::filesystem::ifstream* stream = isEnu ? &enuStream_ : &languageStream_;
    stream->open(path, std::ios::binary);

    unsigned int filesize = boost::filesystem::file_size(path);

    stream->seekg(6);
    uint8_t indexBuffer[7];
    uint32_t id;
    uint16_t len;

    while (stream->tellg() != (std::streampos)filesize) {
        if (stream->tellg() == (std::streampos)-1) {
            LOG_ERROR << "Received value -1 from stream->tellg() in ClilocLoader::indexFile" << std::endl;
            break;
        }
        stream->read(reinterpret_cast<char*>(indexBuffer), 7);
        ClilocEntry curEntry;

        id = indexBuffer[0];
        id |= ((uint32_t)indexBuffer[1]) << 8;
        id |= ((uint32_t)indexBuffer[2]) << 16;
        id |= ((uint32_t)indexBuffer[3]) << 24;

        len = indexBuffer[5];
        len |= ((uint16_t)indexBuffer[6]) << 8;

        curEntry.id_ = id;
        curEntry.len_ = len;
        curEntry.streampos_ = stream->tellg();
        curEntry.fromEnu_ = isEnu;

        if (len > 0) {
            entryMap_[curEntry.id_] = curEntry;

            //LOG_DEBUG << "filesize: " << filesize << " cur=" << stream->tellg() << " len=" << curEntry.len_ << std::hex << " iB[5]=" << (unsigned int)indexBuffer[5] << " iB[6]=" << (unsigned int)indexBuffer[6] << std::dec << std::endl;

            stream->seekg(curEntry.len_, std::ios::cur);
            bufferSize_ = (std::max)(bufferSize_, curEntry.len_);
        }
    }

    if (bufferSize_ > startBufferSize) {
        buffer_ = reinterpret_cast<int8_t*>(realloc(buffer_, bufferSize_));
    }
}

UnicodeString ClilocLoader::readEntry(const ClilocEntry& entry) {
    boost::filesystem::ifstream* stream = entry.fromEnu_ ? &enuStream_ : &languageStream_;

    stream->seekg(entry.streampos_);
    stream->read(reinterpret_cast<char*>(buffer_), entry.len_);

    unsigned int indexHelper = 0;
    UnicodeString ret;
    net::PacketReader::readUtf8Fixed(buffer_, entry.len_, indexHelper, ret, entry.len_);

    unsigned int x = 0;
    ++x;
    x = ret.length();

    return ret;
}


}
}
