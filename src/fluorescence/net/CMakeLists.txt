
set (NET_HPP
    net/manager.hpp
    net/encryption.hpp
    net/xorencryption.hpp
    net/twofishencryption.hpp
    net/noencryption.hpp
    net/decompress.hpp
    net/socket.hpp
    net/packet.hpp
    net/packetlist.hpp
    net/packetwriter.hpp
    net/packetreader.hpp
    net/multipurposepacket.hpp
    net/walkmanager.hpp
    )

set (NET_CPP
    net/manager.cpp
    net/xorencryption.cpp
    net/twofishencryption.cpp
    net/noencryption.cpp
    net/decompress.cpp
    net/socket.cpp
    net/packet.cpp
    net/packetwriter.cpp
    net/packetreader.cpp
    net/multipurposepacket.cpp
    net/walkmanager.cpp
    )

set (PACKETS_HPP
    net/packets/unknown.hpp

    net/packets/loginrequest.hpp
    net/packets/serverlist.hpp
    net/packets/loginreject.hpp
    net/packets/gameserverselect.hpp
    net/packets/serverredirect.hpp
    net/packets/gameserverloginrequest.hpp
    net/packets/characterlist.hpp
    net/packets/characterselect.hpp
    net/packets/playerinit.hpp
    net/packets/logincomplete.hpp
    net/packets/teleport.hpp
    net/packets/nakedmobile.hpp
    net/packets/worlditem.hpp
    net/packets/singleclick.hpp
    net/packets/doubleclick.hpp
    net/packets/deleteobject.hpp
    net/packets/speechrequest.hpp
    net/packets/basetext.hpp
    net/packets/asciitext.hpp
    net/packets/unicodetext.hpp
    net/packets/clientversion.hpp
    net/packets/pickupitem.hpp
    net/packets/dropitem.hpp
    net/packets/pickupreject.hpp
    net/packets/statupdatehitpoints.hpp
    net/packets/statupdatestamina.hpp
    net/packets/statupdatemana.hpp
    net/packets/statskillquery.hpp
    net/packets/statupdatefull.hpp
    net/packets/equippedmobile.hpp
    net/packets/equippeditem.hpp
    net/packets/mobileanimation.hpp
    net/packets/openpaperdoll.hpp
    net/packets/target.hpp
    net/packets/displaycontainer.hpp
    net/packets/containercontent.hpp
    net/packets/containerupdate.hpp
    net/packets/localizedtext.hpp
    net/packets/localizedtextaffix.hpp
    net/packets/movementrequest.hpp
    net/packets/movementaccept.hpp
    net/packets/movementdeny.hpp
    net/packets/createcharacter.hpp
    net/packets/gumpmenu.hpp

    net/packets/bf.hpp
    net/packets/bf/changemap.hpp
    net/packets/bf/contextmenurequest.hpp
    net/packets/bf/opencontextmenu.hpp
    net/packets/bf/contextmenureply.hpp
    net/packets/bf/fastwalkinit.hpp
    net/packets/bf/fastwalkupdate.hpp
    )

set (PACKETS_CPP
    net/packets/unknown.cpp

    net/packets/loginrequest.cpp
    net/packets/serverlist.cpp
    net/packets/loginreject.cpp
    net/packets/gameserverselect.cpp
    net/packets/serverredirect.cpp
    net/packets/gameserverloginrequest.cpp
    net/packets/characterlist.cpp
    net/packets/characterselect.cpp
    net/packets/playerinit.cpp
    net/packets/logincomplete.cpp
    net/packets/teleport.cpp
    net/packets/nakedmobile.cpp
    net/packets/worlditem.cpp
    net/packets/singleclick.cpp
    net/packets/doubleclick.cpp
    net/packets/deleteobject.cpp
    net/packets/speechrequest.cpp
    net/packets/basetext.cpp
    net/packets/asciitext.cpp
    net/packets/unicodetext.cpp
    net/packets/clientversion.cpp
    net/packets/pickupitem.cpp
    net/packets/dropitem.cpp
    net/packets/pickupreject.cpp
    net/packets/statupdatehitpoints.cpp
    net/packets/statupdatestamina.cpp
    net/packets/statupdatemana.cpp
    net/packets/statskillquery.cpp
    net/packets/statupdatefull.cpp
    net/packets/equippedmobile.cpp
    net/packets/equippeditem.cpp
    net/packets/mobileanimation.cpp
    net/packets/openpaperdoll.cpp
    net/packets/target.cpp
    net/packets/displaycontainer.cpp
    net/packets/containercontent.cpp
    net/packets/containerupdate.cpp
    net/packets/localizedtext.cpp
    net/packets/localizedtextaffix.cpp
    net/packets/movementrequest.cpp
    net/packets/movementaccept.cpp
    net/packets/movementdeny.cpp
    net/packets/createcharacter.cpp
    net/packets/gumpmenu.cpp

    net/packets/bf.cpp
    net/packets/bf/changemap.cpp
    net/packets/bf/contextmenurequest.cpp
    net/packets/bf/opencontextmenu.cpp
    net/packets/bf/contextmenureply.cpp
    net/packets/bf/fastwalkinit.cpp
    net/packets/bf/fastwalkupdate.cpp
    )

set (MD5
    net/md5/global.h
    net/md5/md5.h
    net/md5/md5c.c
    )

set (TWOFISH
    net/twofish/aes.h
    net/twofish/debug.h
    net/twofish/platform.h
    net/twofish/table.h
    net/twofish/twofish.c
    )

set(NET_FILES ${NET_HPP} ${NET_CPP} ${MD5} ${TWOFISH} ${PACKETS_HPP} ${PACKETS_CPP} PARENT_SCOPE)

#add_subdirectory(test)
