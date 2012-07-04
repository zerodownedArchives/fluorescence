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



#include "skillsloader.hpp"

#include "fullfileloader.hpp"
#include "indexloader.hpp"

#include <misc/log.hpp>
#include <misc/exception.hpp>

namespace fluo {
namespace data {

SkillsLoader::SkillsLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath) : skillInfos_(nullptr) {
    read(idxPath, mulPath);
}

SkillsLoader::~SkillsLoader() {
    if (skillInfos_) {
        delete [] skillInfos_;
        skillInfos_ = nullptr;
    }
}

void SkillsLoader::read(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath) {
    IndexLoader idxLdr(idxPath);

    boost::filesystem::ifstream stream(mulPath, std::ios_base::binary);
    if (!stream.is_open()) {
        throw Exception("Error opening skills stream");
    }

    unsigned int bufSize = 512;
    char* buf = (char*)malloc(bufSize);

    skillInfos_ = new SkillInfo[idxLdr.size()];

    unsigned int i;
    for (i = 0; i < idxLdr.size(); ++i) {
        IndexBlock curBlock = idxLdr.get(i);

        if (curBlock.offset_ == 0xFFFFFFFF || curBlock.length_ == 0) {
            continue;
        }

        if (curBlock.length_ > bufSize) {
            bufSize *= 2;
            buf = (char*)realloc(buf, bufSize);
        }

        stream.seekg(curBlock.offset_, std::ios::beg);
        stream.read(buf, curBlock.length_);

        skillInfos_[i].skillId_ = i;
        skillInfos_[i].isUsable_ = (buf[0] == 1);
        skillInfos_[i].name_ = StringConverter::fromUtf8(buf + 1, curBlock.length_ - 1);

        LOG_DEBUG << "Skill " << skillInfos_[i].skillId_ << ": " << skillInfos_[i].name_ << " - " << skillInfos_[i].isUsable_ << std::endl;
    }

    skillCount_ = i; // real skill count
    stream.close();
    free(buf);
}

const SkillInfo* SkillsLoader::getSkillInfo(unsigned int id) const {
    if (id >= skillCount_) {
        LOG_WARN << "Trying to access SkillInfo with id " << id << std::endl;
        id = 0;
    }

    return &skillInfos_[id];
}

}
}
