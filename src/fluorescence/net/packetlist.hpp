#ifndef FLUO_NET_PACKETLIST_HPP
#define FLUO_NET_PACKETLIST_HPP

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
#include "packets/logincomplete.hpp" // 0x55
#include "packets/characterselect.hpp" // 0x5d
#include "packets/target.hpp" // 0x6c
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
#include "packets/clientversion.hpp" // 0xbd
#include "packets/bf.hpp" // 0xbf
#include "packets/localizedtext.hpp" // 0xc1
#include "packets/localizedtextaffix.hpp" // 0xcc

#include "packets/unknown.hpp"

#endif
