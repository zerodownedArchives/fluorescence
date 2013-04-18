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



#include "socket.hpp"

#include <errno.h>
#include <string.h>
#ifdef WIN32
#include <winsock2.h>
#include <WS2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#endif

#include <misc/log.hpp>

#include "manager.hpp"
#include "packetwriter.hpp"
#include "packetreader.hpp"

namespace fluo {
namespace net {

Socket::Socket() : socketFd_(0), decompressedSize_(0), sendSize_(0), useDecompress_(false), running_(false) {
}

Socket::~Socket() {
    close();
    receiveThread_.join();
}

void Socket::setEncryption(boost::shared_ptr<Encryption> value) {
    encryption_ = value;
}

bool Socket::connect(const UnicodeString& host, unsigned short port) {
    std::string stdString = StringConverter::toUtf8String(host);

    unsigned int ip = 0;

#ifdef WIN32
    addrinfo hints;
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* result;
    int error = getaddrinfo(stdString.c_str(), NULL, &hints, &result);
    if (error != 0) {
        LOG_ERROR << "Unknown host " << host << ": " << error << " - " << gai_strerror(error) << std::endl;
        close();
        return false;
    }

    ip = ((struct sockaddr_in *)(result->ai_addr))->sin_addr.s_addr;

    freeaddrinfo(result);
#else
    hostent* hostent = gethostbyname(stdString.c_str());

    if (!hostent) {
        LOG_ERROR << "Unknown host " << host << ": " << strerror(errno) << std::endl;
        close();
        return false;
    }

    memcpy(&ip, hostent->h_addr_list[0], 4);
#endif

    return connect(ip, port);
}

bool Socket::connect(unsigned int ip, unsigned short port) {
    if (socketFd_) {
        LOG_ERROR << "Trying to reconnect an already connected socket!" << std::endl;
        return false;
    }

    LOG_INFO << "Trying to connect to " << (ip & 0x000000FF) << "." << ((ip & 0x0000FF00) >> 8) << "." << ((ip & 0x00FF0000) >> 16) <<
            "." << ((ip & 0xFF000000) >> 24) << ":" << port << std::endl;

    socketFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd_ == -1) {
        #ifdef WIN32
            LOG_ERROR << "Unable to open socket: " << strerror(WSAGetLastError()) << std::endl;
        #else
            LOG_ERROR << "Unable to open socket: " << strerror(errno) << std::endl;
        #endif

        return false;
    }

    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = ip;

    if (::connect(socketFd_, (sockaddr*)&dest_addr, sizeof(sockaddr)) == -1) {
        #ifdef WIN32
            LOG_ERROR << "Unable to connect socket: " << strerror(WSAGetLastError()) << std::endl;
        #else
            LOG_ERROR << "Unable to connect socket: " << strerror(errno) << std::endl;
        #endif
        close();
        return false;
    }

    // set socket nonblocking
    #ifdef WIN32
        u_long iMode = 1;
        if (ioctlsocket(socketFd_, FIONBIO, &iMode) != 0) {
            LOG_ERROR << "Unable to set socket to nonblocking: " << strerror(WSAGetLastError()) << std::endl;
            close();
            return false;
        }
    #else
        int flags;

        if ((flags = fcntl(socketFd_, F_GETFL, 0)) < 0 ||
                fcntl(socketFd_, F_SETFL, flags | O_NONBLOCK) < 0) {
            LOG_ERROR << "Unable to set socket to nonblocking: " << strerror(errno) << std::endl;
            close();
            return false;
        }
    #endif

    LOG_INFO << "Socket connected to " << (ip & 0x000000FF) << "." << ((ip & 0x0000FF00) >> 8) << "." << ((ip & 0x00FF0000) >> 16) <<
            ((ip & 0xFF000000) >> 24) << ":" << port << std::endl;

    sendSize_ = 0;
    running_ = true;
    criticalError_ = false;
    useDecompress_ = false;
    decompressedSize_ = 0;

    receiveThread_ = boost::thread(&Socket::receiveRun, this);

    return true;
}

void Socket::close() {
    running_ = false;

    if (socketFd_) {
        LOG_INFO << "Closing socket" << std::endl;

#ifdef WIN32
        closesocket(socketFd_);
#else
        ::close(socketFd_);
#endif
        socketFd_ = 0;
    }

    receiveThread_.join();

    boost::mutex::scoped_lock myLock(packetQueueMutex_);
    while (!packetQueue_.empty()) {
        packetQueue_.pop();
    }
}

void Socket::receiveRun() {
    while (running_ && !criticalError_) {
        // sleep a little
        fluo::sleepMs(10);

        socketMutex_.lock();
        int recvLen = recv(socketFd_, reinterpret_cast<char*>(rawBuffer_), 0x4000, 0);
#ifdef WIN32
        int sockErr = WSAGetLastError();
#endif
        socketMutex_.unlock();

        if (recvLen > 0) {
            // decrypt data
            if (encryption_) {
                //LOG_INFO(LOGTYPE_NETWORK, "decrypt");
                encryption_->decrypt(rawBuffer_, rawBuffer_, recvLen);
            }

            unsigned int decompLen = recvLen;
            if (useDecompress_) {
                // decompress data
                //LOG_INFO(LOGTYPE_NETWORK, "decompress");
                decompLen = decompress_.huffmanDecompress(decompressedBuffer_ + decompressedSize_, 0x10000 - decompressedSize_, rawBuffer_, recvLen);
            } else {
                memcpy(decompressedBuffer_ + decompressedSize_, rawBuffer_, recvLen);
            }

            decompressedSize_ += decompLen;

            //LOG_INFO << "decompressed buffer dump:" << std::endl;
            //dumpBuffer(decompressedBuffer_, decompLen);

            parsePackets();
        } else if (recvLen == 0) { // peer has shut down
            LOG_INFO << "Peer has shut down socket" << std::endl;
            criticalError_ = true;
            break;
        } else if (recvLen < 0 && running_) { // error
#ifdef WIN32
            if (sockErr == EAGAIN || sockErr == WSAEWOULDBLOCK) {
#else
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
#endif
                continue;
            } else {
                #ifdef WIN32
                    LOG_ERROR << "Socket error in receiveRun(): " << sockErr << " - " << strerror(sockErr) << std::endl;
                #else
                    LOG_ERROR << "Socket error in receiveRun(): " << errno << " - " << strerror(errno) << std::endl;
                #endif

                criticalError_ = true;
                break;
            }
        }
    }
}

void Socket::dumpBuffer(int8_t* buf, unsigned int length) {
    for (unsigned int i = 0; i < length; ++i) {
        if (i % 8 == 0) {
            printf("\n%4u: ", i);
        }
        printf("%2x(%c) ", (uint8_t)buf[i], isprint(buf[i]) ? buf[i] : '?');
    }
    printf("\n");
}

bool Socket::sendAll() {
    if (sendSize_ == 0 || criticalError_) {
        return true;
    }

    //LOG_DEBUG(LOGTYPE_NETWORK, "sock send dump");
    //dumpBuffer(sendBuffer_, sendSize_);

    socketMutex_.lock();
#ifdef WIN32
    // no SIGPIPE on windows
    unsigned int sendLen = ::send(socketFd_, reinterpret_cast<char*>(sendBuffer_), sendSize_, 0);
#else
    unsigned int sendLen = ::send(socketFd_, reinterpret_cast<char*>(sendBuffer_), sendSize_, MSG_NOSIGNAL);
#endif
    socketMutex_.unlock();

    if (sendLen != sendSize_) {
        #ifdef WIN32
            LOG_ERROR << "Socket error in sendAll(): " << strerror(WSAGetLastError()) << std::endl;
        #else
            LOG_ERROR << "Socket error in sendAll(): " << strerror(errno) << std::endl;
        #endif

        criticalError_ = true;

        return false;
    }

    sendSize_ = 0;

    return true;
}

void Socket::setSeed(uint32_t seed) {
    seed_ = seed;
}

void Socket::writeShortSeed() {
    PacketWriter::write(sendBuffer_, 0x10000, sendSize_, seed_);
}

uint32_t Socket::getSeed() const {
    return seed_;
}

void Socket::setUseDecompress(bool value) {
    useDecompress_ = value;
}

void Socket::parsePackets() {
    unsigned int idx = 0;
    unsigned int lastPacketStart = 0;
    //LOG_DEBUG << "Parsing packets bufferSize=" << decompressedSize_ << std::endl;

    while ((lastPacketStart = idx) < decompressedSize_) {

        bool readSuccess = true;
        uint8_t packetId;
        readSuccess = PacketReader::read(decompressedBuffer_, decompressedSize_, idx, packetId);

        //LOG_DEBUG << "Parsing packet id=" << std::hex << (unsigned int)packetId << std::dec << std::endl;

        boost::shared_ptr<Packet> newPacket = Manager::createPacket(packetId);

        if (!newPacket) {
            LOG_ERROR << "Unable to create packet for id " << std::hex << (unsigned int)packetId << std::dec << std::endl;
            continue;
        }

        uint16_t packetSize;
        if (newPacket->hasVariableSize()) {
            readSuccess = readSuccess && PacketReader::read(decompressedBuffer_, decompressedSize_, idx, packetSize);
            newPacket->setSize(packetSize);
            //LOG_DEBUG << "Dynamic packet size=" << packetSize << std::endl;
        } else {
            packetSize = newPacket->getSize();
            //LOG_DEBUG << "Fixed packet size=" << packetSize << std::endl;
        }

        if (decompressedSize_ < lastPacketStart + packetSize) {
            //LOG_DEBUG << "Packet not received completely id=" << std::hex << (unsigned int)packetId << std::dec <<
                    //" should-be-len=" << packetSize << " received=" << (decompressedSize_ - lastPacketStart) << std::endl;
            // dumpBuffer(decompressedBuffer_, decompressedSize_);
            break;
        }

        readSuccess = readSuccess && newPacket->read(decompressedBuffer_, lastPacketStart + packetSize, idx);

        if (readSuccess && idx - lastPacketStart == packetSize) {
            //LOG_DEBUG << "Read of packet successful: id=" << std::hex << (unsigned int)packetId << std::dec << " len=" << packetSize << std::endl;

            packetQueueMutex_.lock();
            packetQueue_.push(newPacket);
            packetQueueMutex_.unlock();
        } else if (!readSuccess) {
            LOG_DEBUG << "Not successful reading packet id=" << std::hex << (unsigned int)packetId << std::dec << " len=" << packetSize <<
                    " trying to read more bytes than received" << std::endl;
            // set idx to start of next packet
            idx = lastPacketStart + packetSize;
            dumpBuffer(decompressedBuffer_, decompressedSize_);
        } else {
            LOG_DEBUG << "Not successful reading packet id=" << std::hex << (unsigned int)packetId << std::dec << " len=" << packetSize <<
                    " read bytes=" << (idx - lastPacketStart) << std::endl;
            // set idx to start of next packet
            idx = lastPacketStart + packetSize;
        }
    }

    if (lastPacketStart != decompressedSize_) { // some bytes left
        if (lastPacketStart > 0) {
            // move unread bytes to start of buffer
            memmove(decompressedBuffer_, decompressedBuffer_ + lastPacketStart, decompressedSize_ - lastPacketStart);
            decompressedSize_ -= lastPacketStart;
        }
    } else {
        decompressedSize_ = 0;
    }
}

bool Socket::isConnected() {
    return socketFd_ != -1;
}

boost::shared_ptr<Packet> Socket::getNextPacket() {
    boost::shared_ptr<Packet> ret;

    packetQueueMutex_.lock();
    if (!packetQueue_.empty()) {
        ret = packetQueue_.front();
        packetQueue_.pop();
    }
    packetQueueMutex_.unlock();

    return ret;
}

bool Socket::hasCriticalError() const {
    return criticalError_;
}

}
}
