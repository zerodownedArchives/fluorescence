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


#ifndef FLUO_NET_PACKETS_STATUPDATEFULL_HPP
#define FLUO_NET_PACKETS_STATUPDATEFULL_HPP

#include <typedefs.hpp>

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class StatUpdateFull : public Packet {
public:
    StatUpdateFull();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

private:
    Serial serial_;
    UnicodeString name_;

    uint16_t hitpointsCurrent_;
    uint16_t hitpointsMax_;

    uint8_t nameFlag_;
    uint8_t packetContents_;

    uint8_t gender_;

    uint16_t str_;
    uint16_t dex_;
    uint16_t int_;

    uint16_t staminaCurrent_;
    uint16_t staminaMax_;
    uint16_t manaCurrent_;
    uint16_t manaMax_;

    uint32_t gold_;

    uint16_t resistPhysical_;

    uint16_t weight_;
    uint16_t weightMax_;

    uint8_t race_;

    uint16_t statCap_;

    uint8_t followersCurrent_;
    uint8_t followersMax_;

    uint16_t resistFire_;
    uint16_t resistCold_;
    uint16_t resistPoison_;
    uint16_t resistEnergy_;

    uint16_t luck_;

    uint16_t damageMin_;
    uint16_t damageMax_;

    uint32_t tithingPoints_;

    uint16_t hitChanceIncrease_;
    uint16_t swingSpeedIncrease_;
    uint16_t damageChanceIncrease_;
    uint16_t lowerReagentCost_;
    uint16_t hitPointsRegeneration_;
    uint16_t staminaRegeneration_;
    uint16_t manaRegeneration_;
    uint16_t reflectPhysicalDamage_;
    uint16_t enhancePotions_;
    uint16_t defenseChanceIncrease_;
    uint16_t spellDamageIncrease_;
    uint16_t fasterCastRecovery_;
    uint16_t fasterCasting_;
    uint16_t lowerManaCost_;
    uint16_t strengthIncrease_;
    uint16_t dexterityIncrease_;
    uint16_t intelligenceIncrease_;
    uint16_t hitPointsIncrease_;
    uint16_t staminaIncrease_;
    uint16_t manaIncrease_;
    uint16_t maxHitpointsIncrease_;
    uint16_t maxStaminaIncrease_;
    uint16_t maxManaIncrease_;
};

}
}
}

#endif
