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

#include "packets/00_createcharacter.hpp" // 0x00
#include "packets/02_movementrequest.hpp" // 0x02
#include "packets/05_attackrequest.hpp" // 0x05
#include "packets/06_doubleclick.hpp" // 0x06
#include "packets/07_pickupitem.hpp" // 0x07
#include "packets/08_dropitem.hpp" // 0x08
#include "packets/09_singleclick.hpp" // 0x09
#include "packets/11_statupdatefull.hpp" //0x11
#include "packets/12_useskill.hpp" //0x12
#include "packets/13_equiprequest.hpp" //0x13
#include "packets/1a_worlditem.hpp" // 0x1a
#include "packets/1b_playerinit.hpp" // 0x1b
#include "packets/1c_asciitext.hpp" // 0x1c
#include "packets/1d_deleteobject.hpp" // 0x1d
#include "packets/20_teleport.hpp" // 0x20
#include "packets/21_movementdeny.hpp" // 0x21
#include "packets/22_movementaccept.hpp" // 0x22
#include "packets/24_displaycontainer.hpp" // 0x24
#include "packets/25_containerupdate.hpp" // 0x25
#include "packets/27_pickupreject.hpp" // 0x27
#include "packets/2e_equippeditem.hpp" // 0x2e
#include "packets/34_statskillquery.hpp" //0x34
#include "packets/3a_skilllockchange.hpp" //0x3a, sent by client
#include "packets/3a_skillsupdate.hpp" //0x3a, sent by server
#include "packets/3c_containercontent.hpp" // 0x3c
#include "packets/54_playsound.hpp" // 0x54
#include "packets/55_logincomplete.hpp" // 0x55
#include "packets/5d_characterselect.hpp" // 0x5d
#include "packets/6c_target.hpp" // 0x6c
#include "packets/6d_playmusic.hpp" // 0x6d
#include "packets/6e_mobileanimation.hpp" // 0x6e
#include "packets/72_warmode.hpp" // 0x72
#include "packets/77_nakedmobile.hpp" // 0x77
#include "packets/78_equippedmobile.hpp" // 0x78
#include "packets/80_loginrequest.hpp" // 0x80
#include "packets/82_loginreject.hpp" // 0x82
#include "packets/83_characterdelete.hpp" // 0x83
#include "packets/85_characterdeletereject.hpp" // 0x85
#include "packets/86_characterlistupdate.hpp" // 0x86
#include "packets/88_openpaperdoll.hpp" //0x88
#include "packets/8c_serverredirect.hpp" // 0x8c
#include "packets/91_gameserverloginrequest.hpp" // 0x91
#include "packets/a0_gameserverselect.hpp" // 0xA0
#include "packets/a1_statupdatehitpoints.hpp" // 0xA1
#include "packets/a2_statupdatemana.hpp" // 0xA2
#include "packets/a3_statupdatestamina.hpp" // 0xA3
#include "packets/a8_serverlist.hpp" // 0xA8
#include "packets/a9_characterlist.hpp" // 0xa9
#include "packets/ad_speechrequest.hpp" // 0xad
#include "packets/ae_unicodetext.hpp" // 0xae
#include "packets/b0_gumpmenu.hpp" // 0xb0
#include "packets/b1_gumpreply.hpp" // 0xb1
#include "packets/b8_profile.hpp" // 0xb8
#include "packets/b9_helprequest.hpp" // 0xb9
#include "packets/bd_clientversion.hpp" // 0xbd
#include "packets/bf.hpp" // 0xbf
#include "packets/c0_osieffect.hpp" // 0xc0
#include "packets/c1_localizedtext.hpp" // 0xc1
#include "packets/c7_osieffectextended.hpp" // 0xc7
#include "packets/c8_updaterange.hpp" // 0xc8
#include "packets/cc_localizedtextaffix.hpp" // 0xcc

#include "packets/unknown.hpp"

#endif
