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


#ifndef FLUO_DATA_SKILLSLOADER_HPP
#define FLUO_DATA_SKILLSLOADER_HPP

#include <misc/string.hpp>

#include <boost/filesystem.hpp>

namespace fluo {
namespace data {

struct SkillInfo {
    uint32_t skillId_;
    UnicodeString name_;
    bool isUsable_;
};

class SkillsLoader {
public:
    SkillsLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath);
    ~SkillsLoader();

    // 0-based
    const SkillInfo* getSkillInfo(unsigned int id) const;
    unsigned int getSkillCount() const;

    void read(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath);

private:
    unsigned int skillCount_;
    SkillInfo* skillInfos_;
};

}
}

#endif
