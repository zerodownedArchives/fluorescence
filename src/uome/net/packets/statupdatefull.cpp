
#include "statupdatefull.hpp"

namespace uome {
namespace net {
namespace packets {

StatUpdateFull::StatUpdateFull() : Packet(0x11) {
}

bool StatUpdateFull::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::readUtf8Fixed(buf, len, index, name_, 30);
    ret &= PacketReader::read(buf, len, index, hitpointsCurrent_);
    ret &= PacketReader::read(buf, len, index, hitpointsMax_);
    ret &= PacketReader::read(buf, len, index, nameFlag_);
    ret &= PacketReader::read(buf, len, index, packetContents_);

    if (packetContents_ > 0) {
        ret &= PacketReader::read(buf, len, index, gender_);
        ret &= PacketReader::read(buf, len, index, str_);
        ret &= PacketReader::read(buf, len, index, dex_);
        ret &= PacketReader::read(buf, len, index, int_);
        ret &= PacketReader::read(buf, len, index, staminaCurrent_);
        ret &= PacketReader::read(buf, len, index, staminaMax_);
        ret &= PacketReader::read(buf, len, index, manaCurrent_);
        ret &= PacketReader::read(buf, len, index, manaMax_);
        ret &= PacketReader::read(buf, len, index, gold_);
        ret &= PacketReader::read(buf, len, index, resistPhysical_);
        ret &= PacketReader::read(buf, len, index, weight_);
    }

    if (packetContents_ >= 5) {
        ret &= PacketReader::read(buf, len, index,  weightMax_);
        ret &= PacketReader::read(buf, len, index, race_);
    }

    if (packetContents_ >= 3) {
        ret &= PacketReader::read(buf, len, index, statCap_);
        ret &= PacketReader::read(buf, len, index, followersCurrent_);
        ret &= PacketReader::read(buf, len, index, followersMax_);
    }

    if (packetContents_ >= 4) {
        ret &= PacketReader::read(buf, len, index, resistFire_);
        ret &= PacketReader::read(buf, len, index, resistCold_);
        ret &= PacketReader::read(buf, len, index, resistPoison_);
        ret &= PacketReader::read(buf, len, index, resistEnergy_);
        ret &= PacketReader::read(buf, len, index, luck_);
        ret &= PacketReader::read(buf, len, index, damageMin_);
        ret &= PacketReader::read(buf, len, index, damageMax_);
        ret &= PacketReader::read(buf, len, index, tithingPoints_);
    }

    if (packetContents_ >= 6) {
        ret &= PacketReader::read(buf, len, index, hitChanceIncrease_);
        ret &= PacketReader::read(buf, len, index, swingSpeedIncrease_);
        ret &= PacketReader::read(buf, len, index, damageChanceIncrease_);
        ret &= PacketReader::read(buf, len, index, lowerReagentCost_);
        ret &= PacketReader::read(buf, len, index, hitPointsRegeneration_);
        ret &= PacketReader::read(buf, len, index, staminaRegeneration_);
        ret &= PacketReader::read(buf, len, index, manaRegeneration_);
        ret &= PacketReader::read(buf, len, index, reflectPhysicalDamage_);
        ret &= PacketReader::read(buf, len, index, enhancePotions_);
        ret &= PacketReader::read(buf, len, index, defenseChanceIncrease_);
        ret &= PacketReader::read(buf, len, index, spellDamageIncrease_);
        ret &= PacketReader::read(buf, len, index, fasterCastRecovery_);
        ret &= PacketReader::read(buf, len, index, fasterCasting_);
        ret &= PacketReader::read(buf, len, index, lowerManaCost_);
        ret &= PacketReader::read(buf, len, index, strengthIncrease_);
        ret &= PacketReader::read(buf, len, index, dexterityIncrease_);
        ret &= PacketReader::read(buf, len, index, intelligenceIncrease_);
        ret &= PacketReader::read(buf, len, index, hitPointsIncrease_);
        ret &= PacketReader::read(buf, len, index, staminaIncrease_);
        ret &= PacketReader::read(buf, len, index, manaIncrease_);
        ret &= PacketReader::read(buf, len, index, maxHitpointsIncrease_);
        ret &= PacketReader::read(buf, len, index, maxStaminaIncrease_);
        ret &= PacketReader::read(buf, len, index, maxManaIncrease_);
    }

    return ret;
}

void StatUpdateFull::onReceive() {
}

}
}
}
