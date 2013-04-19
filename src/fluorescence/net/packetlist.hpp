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


#ifndef FLUO_NET_PACKETLIST_HPP
#define FLUO_NET_PACKETLIST_HPP

#include "packets/00_createcharacter.hpp"
#include "packets/02_movementrequest.hpp"
#include "packets/05_attackrequest.hpp"
#include "packets/06_doubleclick.hpp"
#include "packets/07_pickupitem.hpp"
#include "packets/08_dropitem.hpp"
#include "packets/09_singleclick.hpp"
#include "packets/11_statupdatefull.hpp"
#include "packets/12_useskill.hpp"
#include "packets/13_equiprequest.hpp"
#include "packets/1a_worlditem.hpp" // deprecated since SA, replaced by f3
#include "packets/1b_playerinit.hpp"
#include "packets/1c_asciitext.hpp"
#include "packets/1d_deleteobject.hpp"
#include "packets/20_teleport.hpp"
#include "packets/21_movementdeny.hpp"
#include "packets/22_movementaccept.hpp"
#include "packets/24_displaycontainer.hpp"
#include "packets/25_containerupdate.hpp"
#include "packets/27_pickupreject.hpp"
#include "packets/2e_equippeditem.hpp"
#include "packets/34_statskillquery.hpp"
#include "packets/3a_skilllockchange.hpp" // sent by client
#include "packets/3a_skillsupdate.hpp" // sent by server
#include "packets/3c_containercontent.hpp"
#include "packets/54_playsound.hpp"
#include "packets/55_logincomplete.hpp"
#include "packets/5d_characterselect.hpp"
#include "packets/65_weather.hpp"
#include "packets/6c_target.hpp"
#include "packets/6d_playmusic.hpp"
#include "packets/6e_mobileanimation.hpp" // deprecated since SA, replaced by e2
#include "packets/72_warmode.hpp"
#include "packets/73_ping.hpp"
#include "packets/77_nakedmobile.hpp"
#include "packets/78_equippedmobile.hpp"
#include "packets/7c_objectpicker.hpp"
#include "packets/7d_objectpickerresponse.hpp"
#include "packets/80_loginrequest.hpp"
#include "packets/82_loginreject.hpp"
#include "packets/83_characterdelete.hpp"
#include "packets/85_characterdeletereject.hpp"
#include "packets/86_characterlistupdate.hpp"
#include "packets/88_openpaperdoll.hpp"
#include "packets/8c_serverredirect.hpp"
#include "packets/91_gameserverloginrequest.hpp"
#include "packets/9b_helprequest.hpp"
#include "packets/9e_vendorselllist.hpp"
#include "packets/9f_vendorselloffer.hpp"
#include "packets/a0_gameserverselect.hpp"
#include "packets/a1_statupdatehitpoints.hpp"
#include "packets/a2_statupdatemana.hpp"
#include "packets/a3_statupdatestamina.hpp"
#include "packets/a8_serverlist.hpp"
#include "packets/a9_characterlist.hpp"
#include "packets/ad_speechrequest.hpp"
#include "packets/ae_unicodetext.hpp"
#include "packets/b0_gumpmenu.hpp"
#include "packets/b1_gumpreply.hpp"
#include "packets/b8_profile.hpp"
#include "packets/bd_clientversion.hpp"
#include "packets/bf.hpp"
#include "packets/c0_osieffect.hpp"
#include "packets/c1_localizedtext.hpp"
#include "packets/c2_unicodeprompt.hpp"
#include "packets/c2_unicodepromptreply.hpp"
#include "packets/c7_osieffectextended.hpp"
#include "packets/c8_updaterange.hpp"
#include "packets/cc_localizedtextaffix.hpp"
#include "packets/d6_objectpropertyrequest.hpp"
#include "packets/d6_objectproperties.hpp"
#include "packets/dc_objectpropertieshash.hpp"
#include "packets/dd_compressedgumpmenu.hpp"
#include "packets/df_buff.hpp"
#include "packets/e2_mobileanimation_sa.hpp" // replaces 6e
#include "packets/ef_seed.hpp"
#include "packets/f3_worldobject.hpp" // replaces 1a

#include "packets/unknown.hpp"

#endif
