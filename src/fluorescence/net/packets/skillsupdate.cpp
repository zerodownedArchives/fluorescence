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
#include <data/manager.hpp>
#include <data/skillsloader.hpp>
#include <world/manager.hpp>
#include <world/mobile.hpp>

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
            SkillValue newVal = readValue(buf, len, index, false, true);
            if (newVal.skillId_ == 0) {
                break;
            }
            skillValues_.push_back(newVal);
        }
        break;
    }
    case 0x02:
        // skill list with caps
        while (true) {
            SkillValue newVal = readValue(buf, len, index, true, true);
            if (newVal.skillId_ == 0) {
                break;
            }
            skillValues_.push_back(newVal);
        }
        break;
    case 0xDF: {
        // single skill update with cap
        SkillValue newVal = readValue(buf, len, index, true, false);
        skillValues_.push_back(newVal);
        break;
    }
    case 0xFF: {
        // single skill update without cap
        SkillValue newVal = readValue(buf, len, index, false, false);
        skillValues_.push_back(newVal);
        break;
    }
    }

    return ret;
}

SkillValue SkillsUpdate::readValue(const int8_t* buf, unsigned int len, unsigned int& index, bool readMaxValue, bool stopOnZero) const {
    SkillValue val;

    PacketReader::read(buf, len, index, val.skillId_);

    if (stopOnZero && val.skillId_ == 0) {
        return val;
    }

    if (!stopOnZero) {
        // skill id is 0-based in this case => change it to be the same as in the full update packet
        val.skillId_ += 1;
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
    boost::shared_ptr<world::Mobile> player = world::Manager::getSingleton()->getPlayer();
    if (!player) {
        LOG_ERROR << "Skills update without player" << std::endl;
        return;
    }

    //LOG_DEBUG << "Skills update for " << skillValues_.size() << " skills:" << std::endl;

    std::list<SkillValue>::const_iterator iter = skillValues_.begin();
    std::list<SkillValue>::const_iterator end = skillValues_.end();
    for (; iter != end; ++iter) {
        UnicodeString propNameBase("skills.");
        propNameBase += data::Manager::getSkillsLoader()->getSkillInfo(iter->skillId_ - 1)->name_;

        UnicodeString propNameCur(propNameBase);
        propNameCur += ".value";

        if (player->hasProperty(propNameCur)) {
            Variable& skillValue = player->getProperty(propNameCur);
            float diff = (iter->value_ - skillValue.asInt()) / 10.0;
            // check skill changed => syslog message
            if (diff > 0) {
                std::stringstream sstr;
                sstr << "Your skill in " << data::Manager::getSkillsLoader()->getSkillInfo(iter->skillId_ - 1)->name_
                        << " has increased by " << diff << "%. It is now " << (iter->value_ / 10.f) << "%.";
                UnicodeString msg(sstr.str().c_str());
                world::Manager::getSingleton()->systemMessage(msg, 86);
            } else if (diff < 0) {
                std::stringstream sstr;
                sstr << "Your skill in " << data::Manager::getSkillsLoader()->getSkillInfo(iter->skillId_ - 1)->name_
                        << " has decreased by " << (-diff) << "%. It is now " << (iter->value_ / 10.f) << "%.";
                UnicodeString msg(sstr.str().c_str());
                world::Manager::getSingleton()->systemMessage(msg, 86);
            }
            skillValue.setInt(iter->value_);
        } else {
            player->getProperty(propNameCur).setInt(iter->value_);
        }

        propNameCur = propNameBase;
        propNameCur += ".base";
        player->getProperty(propNameCur).setInt(iter->baseValue_);

        propNameCur = propNameBase;
        propNameCur += ".cap";
        player->getProperty(propNameCur).setInt(iter->maxValue_);

        propNameCur = propNameBase;
        propNameCur += ".lock";
        player->getProperty(propNameCur).setInt(iter->lockStatus_);

        //LOG_DEBUG << "Skill " << iter->skillId_ << ": "
                //<< data::Manager::getSkillsLoader()->getSkillInfo(iter->skillId_ - 1)->name_ << " "
                //<< iter->value_ << "/" << iter->baseValue_ << "/" << iter->maxValue_ << " lock="
                //<< (unsigned int)iter->lockStatus_ << std::endl;
    }

    player->onPropertyUpdate();
}

}
}
}
