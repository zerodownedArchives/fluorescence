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



#include "skillsupdate.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

SkillsUpdate::SkillsUpdate() : Packet(0x3A) {
}

bool SkillsUpdate::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, type_);

    switch (type_) {
    case 0x00: {
        // skill list without caps
        while (true) {
            SkillValue newVal = readValue(buf, len, index, false);
            if (newVal.skillId_ == 0) {
                break;
            }
            skillValues_.push_back(newVal);
        }
        break;
    }
    case 0x02:
        // skill list with caps
        // skill list without caps
        while (true) {
            SkillValue newVal = readValue(buf, len, index, true);
            if (newVal.skillId_ == 0) {
                break;
            }
            skillValues_.push_back(newVal);
        }
        break;
    case 0xDF: {
        // single skill update without cap
        SkillValue newVal = readValue(buf, len, index, true);
        skillValues_.push_back(newVal);
        break;
    }
    case 0xFF: {
        // single skill update without cap
        SkillValue newVal = readValue(buf, len, index, false);
        skillValues_.push_back(newVal);
        break;
    }
    }

    return ret;
}

SkillValue SkillsUpdate::readValue(const int8_t* buf, unsigned int len, unsigned int& index, bool readMaxValue) const {
    SkillValue val;

    PacketReader::read(buf, len, index, val.skillId_);

    if (val.skillId_ == 0) {
        return val;
    }

    PacketReader::read(buf, len, index, val.value_);
    PacketReader::read(buf, len, index, val.baseValue_);
    PacketReader::read(buf, len, index, val.lockStatus_);

    if (readMaxValue) {
        PacketReader::read(buf, len, index, val.maxValue_);
    } else {
        val.maxValue_ = 0;
    }

    return val;
}

void SkillsUpdate::onReceive() {
    LOG_DEBUG << "Skills update for " << skillValues_.size() << " skills:" << std::endl;
    std::list<SkillValue>::const_iterator iter = skillValues_.begin();
    std::list<SkillValue>::const_iterator end = skillValues_.end();
    for (; iter != end; ++iter) {
        LOG_DEBUG << "Skill " << iter->skillId_ << ": " << iter->value_ << "/" << iter->baseValue_ << "/" << iter->maxValue_ << " lock=" << (unsigned int)iter->lockStatus_ << std::endl;
    }
}

}
}
}
