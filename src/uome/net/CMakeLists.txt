
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
