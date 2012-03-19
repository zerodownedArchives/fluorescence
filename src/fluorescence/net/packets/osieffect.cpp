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



#include "osieffect.hpp"

#include <boost/shared_ptr.hpp>

#include <world/manager.hpp>
#include <world/osieffect.hpp>
#include <world/mobile.hpp>
#include <world/dynamicitem.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

OsiEffect::OsiEffect(uint8_t packetId, uint16_t len) : Packet(packetId, len) {
}

bool OsiEffect::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;
    
    ret &= PacketReader::read(buf, len, index, effectType_);
    ret &= PacketReader::read(buf, len, index, sourceSerial_);
    ret &= PacketReader::read(buf, len, index, targetSerial_);
    ret &= PacketReader::read(buf, len, index, artId_);
    ret &= PacketReader::read(buf, len, index, sourceX_);
    ret &= PacketReader::read(buf, len, index, sourceY_);
    ret &= PacketReader::read(buf, len, index, sourceZ_);
    ret &= PacketReader::read(buf, len, index, targetX_);
    ret &= PacketReader::read(buf, len, index, targetY_);
    ret &= PacketReader::read(buf, len, index, targetZ_);
    ret &= PacketReader::read(buf, len, index, speed_);
    ret &= PacketReader::read(buf, len, index, duration_);
    
    // jump 2 unknown bytes
    index += 2;
    
    ret &= PacketReader::read(buf, len, index, fixedDirection_);
    ret &= PacketReader::read(buf, len, index, explode_);
    ret &= PacketReader::read(buf, len, index, hue_);
    ret &= PacketReader::read(buf, len, index, renderMode_);
    
    return ret;
}

void OsiEffect::onReceive() {
    world::Manager* worldMan = world::Manager::getSingleton();
    
    boost::shared_ptr<world::IngameObject> sourceObj = worldMan->getMobile(sourceSerial_, false);
    if (!sourceObj) {
        sourceObj = boost::static_pointer_cast<world::IngameObject>(worldMan->getDynamicItem(sourceSerial_, false));
    }
    
    boost::shared_ptr<world::IngameObject> targetObj = boost::static_pointer_cast<world::IngameObject>(worldMan->getMobile(targetSerial_, false));
    if (!targetObj) {
        targetObj = boost::static_pointer_cast<world::IngameObject>(worldMan->getDynamicItem(targetSerial_, false));
    }
    
    //LOG_DEBUG << "Effect src!=NULL: " << (bool)sourceObj << " targ!=NULL:" << (bool)targetObj << " duration=" << (unsigned int)duration_ << " speed=" << (unsigned int)speed_ << std::endl;
    
    boost::shared_ptr<world::Effect> effect(new world::OsiEffect(artId_));
    
    switch (effectType_) {
        case world::Effect::TYPE_SOURCE_TO_TARGET:
            effect->setMoving(sourceObj, targetObj, speed_);
            break;
            
        case world::Effect::TYPE_LIGHTNING:
            effect->setLightning(sourceObj, targetObj);
            break;
            
        case world::Effect::TYPE_AT_POSITION:
            effect->setAtPosition(sourceX_, sourceY_, sourceZ_);
            break;
            
        case world::Effect::TYPE_AT_SOURCE:
            effect->setAtSource(sourceObj);
            break;
    }
    
    effect->setLifetimeMillis(duration_ * 50);
    effect->setShouldExplode(explode_ != 0);
    effect->setFixedAngle(fixedDirection_ != 0);
    
    worldMan->addEffect(effect);
}

}
}
}
