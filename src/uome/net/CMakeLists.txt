
set (NET_HPP
    manager.hpp
    encryption.hpp
    xorencryption.hpp
    twofishencryption.hpp
    noencryption.hpp
    decompress.hpp
    socket.hpp
    packet.hpp
    packetwriter.hpp
    packetreader.hpp
    )

set (NET_CPP
    manager.cpp
    xorencryption.cpp
    twofishencryption.cpp
    noencryption.cpp
    decompress.cpp
    socket.cpp
    packet.cpp
    packetwriter.cpp
    packetreader.cpp
    )

set (PACKETS_HPP
    packets/loginrequest.hpp
    )

set (PACKETS_CPP
    packets/loginrequest.cpp
    )

set (MD5
    md5/global.h
    md5/md5.h
    md5/md5c.c
    )

set (TWOFISH
    twofish/aes.h
    twofish/debug.h
    twofish/platform.h
    twofish/table.h
    twofish/twofish.c
    )

add_library(uomenet STATIC ${NET_HPP} ${NET_CPP} ${MD5} ${TWOFISH} ${PACKETS_HPP} ${PACKETS_CPP})

#add_subdirectory(test)
