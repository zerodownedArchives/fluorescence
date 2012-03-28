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

#include "packets/createcharacter.hpp" // 0x00
#include "packets/movementrequest.hpp" // 0x02
#include "packets/doubleclick.hpp" // 0x06
#include "packets/pickupitem.hpp" // 0x07
#include "packets/dropitem.hpp" // 0x08
#include "packets/singleclick.hpp" // 0x09
#include "packets/statupdatefull.hpp" //0x11
#include "packets/worlditem.hpp" // 0x1a
#include "packets/playerinit.hpp" // 0x1b
#include "packets/asciitext.hpp" // 0x1c
#include "packets/deleteobject.hpp" // 0x1d
#include "packets/teleport.hpp" // 0x20
#include "packets/movementdeny.hpp" // 0x21
#include "packets/movementaccept.hpp" // 0x22
#include "packets/displaycontainer.hpp" // 0x24
#include "packets/containerupdate.hpp" // 0x25
#include "packets/pickupreject.hpp" // 0x27
#include "packets/equippeditem.hpp" // 0x2e
#include "packets/statskillquery.hpp" //0x34
#include "packets/containercontent.hpp" // 0x3c
#include "packets/playsound.hpp" // 0x54
#include "packets/logincomplete.hpp" // 0x55
#include "packets/characterselect.hpp" // 0x5d
#include "packets/target.hpp" // 0x6c
#include "packets/playmusic.hpp" // 0x6d
#include "packets/mobileanimation.hpp" // 0x6e
#include "packets/nakedmobile.hpp" // 0x77
#include "packets/equippedmobile.hpp" // 0x78
#include "packets/loginrequest.hpp" // 0x80
#include "packets/loginreject.hpp" // 0x82
#include "packets/openpaperdoll.hpp" //0x88
#include "packets/serverredirect.hpp" // 0x8c
#include "packets/gameserverloginrequest.hpp" // 0x91
#include "packets/gameserverselect.hpp" // 0xA0
#include "packets/statupdatehitpoints.hpp" // 0xA1
#include "packets/statupdatemana.hpp" // 0xA2
#include "packets/statupdatestamina.hpp" // 0xA3
#include "packets/serverlist.hpp" // 0xA8
#include "packets/characterlist.hpp" // 0xa9
#include "packets/speechrequest.hpp" // 0xad
#include "packets/unicodetext.hpp" // 0xae
#include "packets/gumpmenu.hpp" // 0xb0
#include "packets/gumpreply.hpp" // 0xb1
#include "packets/clientversion.hpp" // 0xbd
#include "packets/bf.hpp" // 0xbf
#include "packets/osieffect.hpp" // 0xc0
#include "packets/localizedtext.hpp" // 0xc1
#include "packets/osieffectextended.hpp" // 0xc7
#include "packets/updaterange.hpp" // 0xc8
#include "packets/localizedtextaffix.hpp" // 0xcc

#include "packets/unknown.hpp"

#endif
