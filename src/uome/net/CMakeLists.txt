
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
    )

set (PACKETS_CPP
    net/packets/unknown.cpp

    net/packets/loginrequest.cpp
    net/packets/serverlist.cpp
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
