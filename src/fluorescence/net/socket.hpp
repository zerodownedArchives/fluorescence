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


#ifndef FLUO_NET_SOCKET_HPP
#define FLUO_NET_SOCKET_HPP

#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <queue>

#include <misc/string.hpp>
#include "encryption.hpp"
#include "decompress.hpp"
#include "packet.hpp"

namespace fluo {
namespace net {

class Socket {
public:
    Socket();
    ~Socket();

    void setEncryption(boost::shared_ptr<Encryption> value);

    bool connect(const UnicodeString& host, unsigned short port);
    bool connect(unsigned int ip, unsigned short port);
    void close();

    bool isConnected();

    template<class P>
    bool write(const P& packet) {
        return packet.write(sendBuffer_, 0x10000, sendSize_);
    }

    void setSeed(uint32_t seed);
    uint32_t getSeed() const;
    void writeShortSeed();

    bool sendAll();

    void setUseDecompress(bool value);

    boost::shared_ptr<Packet> getNextPacket();

    bool hasCriticalError() const;

private:
    int socketFd_;

    boost::mutex socketMutex_;

    int8_t rawBuffer_[0x4000];

    int8_t decompressedBuffer_[0x10000];
    unsigned int decompressedSize_;

    int8_t sendBuffer_[0x10000];
    unsigned int sendSize_;

    uint32_t seed_;
    boost::shared_ptr<Encryption> encryption_;

    Decompress decompress_;
    bool useDecompress_;

    bool running_;
    boost::thread receiveThread_;
    void receiveRun();

    bool criticalError_;

    void dumpBuffer(int8_t* buffer, unsigned int length);


    boost::mutex packetQueueMutex_;
    std::queue<boost::shared_ptr<Packet> > packetQueue_;
    void parsePackets();
};

}
}

#endif
