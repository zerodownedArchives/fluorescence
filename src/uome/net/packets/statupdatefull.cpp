
#include "statupdatefull.hpp"

#include <misc/log.hpp>

#include <world/manager.hpp>
#include <world/mobile.hpp>

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
    boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(serial_, false);
    if (!mob) {
        LOG_WARN << "Received full stat update for unknown mobile serial=" << serial_ << std::endl;
        return;
    }

    mob->getProperty("name").setString(name_);
    mob->getProperty("hitpoints").setInt(hitpointsCurrent_);
    mob->getProperty("hitpoints-max").setInt(hitpointsMax_);

    // TODO: handle name change
    //mob->getProperty().setInt("", nameFlag_);

    if (packetContents_ > 0) {
        mob->getProperty("gender").setInt(gender_);
        mob->getProperty("strength").setInt(str_);
        mob->getProperty("dexterity").setInt(dex_);
        mob->getProperty("intelligence").setInt(int_);
        mob->getProperty("stamina").setInt(staminaCurrent_);
        mob->getProperty("stamina-max").setInt(staminaMax_);
        mob->getProperty("mana").setInt(manaCurrent_);
        mob->getProperty("mana-max").setInt(manaMax_);
        mob->getProperty("gold").setInt(gold_);
        mob->getProperty("resist-physical").setInt(resistPhysical_);
        mob->getProperty("armor-rating").setInt(resistPhysical_);
        mob->getProperty("weight").setInt(weight_);
    }

    if (packetContents_ >= 5) {
        mob->getProperty("weight-max").setInt( weightMax_);
        mob->getProperty("race").setInt(race_);
    }

    if (packetContents_ >= 3) {
        mob->getProperty("statcap").setInt(statCap_);
        mob->getProperty("followers").setInt(followersCurrent_);
        mob->getProperty("followers-max").setInt(followersMax_);
    }

    if (packetContents_ >= 4) {
        mob->getProperty("resist-fire").setInt(resistFire_);
        mob->getProperty("resist-cold").setInt(resistCold_);
        mob->getProperty("resist-poison").setInt(resistPoison_);
        mob->getProperty("resist-energy").setInt(resistEnergy_);
        mob->getProperty("luck").setInt(luck_);
        mob->getProperty("damage-min").setInt(damageMin_);
        mob->getProperty("damage-max").setInt(damageMax_);
        mob->getProperty("tithing-points").setInt(tithingPoints_);
    }

    if (packetContents_ >= 6) {
        LOG_WARN << "Received KR info in packet 0x11, unable to handle that" << std::endl;
        //mob->getProperty("").setInt(hitChanceIncrease_);
        //mob->getProperty("").setInt(swingSpeedIncrease_);
        //mob->getProperty("").setInt(damageChanceIncrease_);
        //mob->getProperty("").setInt(lowerReagentCost_);
        //mob->getProperty("").setInt(hitPointsRegeneration_);
        //mob->getProperty("").setInt(staminaRegeneration_);
        //mob->getProperty("").setInt(manaRegeneration_);
        //mob->getProperty("").setInt(reflectPhysicalDamage_);
        //mob->getProperty("").setInt(enhancePotions_);
        //mob->getProperty("").setInt(defenseChanceIncrease_);
        //mob->getProperty("").setInt(spellDamageIncrease_);
        //mob->getProperty("").setInt(fasterCastRecovery_);
        //mob->getProperty("").setInt(fasterCasting_);
        //mob->getProperty("").setInt(lowerManaCost_);
        //mob->getProperty("").setInt(strengthIncrease_);
        //mob->getProperty("").setInt(dexterityIncrease_);
        //mob->getProperty("").setInt(intelligenceIncrease_);
        //mob->getProperty("").setInt(hitPointsIncrease_);
        //mob->getProperty("").setInt(staminaIncrease_);
        //mob->getProperty("").setInt(manaIncrease_);
        //mob->getProperty("").setInt(maxHitpointsIncrease_);
        //mob->getProperty("").setInt(maxStaminaIncrease_);
        //mob->getProperty("").setInt(maxManaIncrease_);
    }

    mob->onPropertyUpdate();
}

}
}
}
