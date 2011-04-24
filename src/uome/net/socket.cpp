
#include "socket.hpp"

#include <errno.h>
#include <string.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#endif

#include <misc/logger.hpp>

#include "packetwriter.hpp"

namespace uome {
namespace net {

Socket::Socket() : socketFd_(0), decompressedIndex_(0), sendIndex_(0), useDecompress_(false), running_(false) {
}

Socket::~Socket() {
    close();
    receiveThread_.join();
}

void Socket::setEncryption(boost::shared_ptr<Encryption> value) {
    encryption_ = value;
}

bool Socket::connect(const std::string& host, unsigned short port) {
    if (socketFd_) {
        LOG_ERROR(LOGTYPE_NETWORK, "Trying to reconnect an already connected socket!");
        return false;
    }

    socketFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd_ == -1) {
        #ifdef WIN32
            LOGARG_ERROR(LOGTYPE_NETWORK, "Unable to open socket: %s", strerror(WSAGetLastError()));
        #else
            LOGARG_ERROR(LOGTYPE_NETWORK, "Unable to open socket: %s", strerror(errno));
        #endif

        return false;
    }

    hostent* hostent = gethostbyname(host.c_str());

    if (!hostent) {
        LOGARG_ERROR(LOGTYPE_NETWORK, "Unknown host: %s", host.c_str());
        return false;
    }

    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    memcpy(&(dest_addr.sin_addr), hostent->h_addr_list[0], 4);

    if (::connect(socketFd_, (sockaddr*)&dest_addr, sizeof(sockaddr)) == -1) {
        #ifdef WIN32
            LOGARG_ERROR(LOGTYPE_NETWORK, "Unable to connect socket: %s", strerror(WSAGetLastError()));
        #else
            LOGARG_ERROR(LOGTYPE_NETWORK, "Unable to connect socket: %s", strerror(errno));
        #endif
        return false;
    }

    // set socket nonblocking
    #ifdef WIN32
        u_long iMode = 1;
        if (ioctlsocket(m_sock, FIONBIO, &iMode) != 0) {
            LOGARG_ERROR(LOGTYPE_NETWORK, "Unable to set socket to nonblocking: %s", strerror(WSAGetLastError()));
            return false;
        }
    #else
        int flags;

        if ((flags = fcntl(socketFd_, F_GETFL, 0)) < 0 ||
                fcntl(socketFd_, F_SETFL, flags | O_NONBLOCK) < 0) {
            LOGARG_ERROR(LOGTYPE_NETWORK, "Unable to set socket to nonblocking: %s", strerror(errno));
            return false;
        }
    #endif


    sendIndex_ = 0;
    running_ = true;
    criticalError_ = false;

    receiveThread_ = boost::thread(&Socket::receiveRun, this);

    return true;
}

void Socket::close() {
    running_ = false;

    if (socketFd_) {
        LOG_INFO(LOGTYPE_NETWORK, "Closing socket");
        ::close(socketFd_);
        socketFd_ = 0;
    }
}

void Socket::receiveRun() {
    while (running_ && !criticalError_) {
        socketMutex_.lock();
        int recvLen = recv(socketFd_, rawBuffer_, 0x4000, 0);
        socketMutex_.unlock();

        if (recvLen > 0) {
            // decrypt data
            if (encryption_) {
                LOG_INFO(LOGTYPE_NETWORK, "decrypt");
                encryption_->decrypt(rawBuffer_, rawBuffer_, recvLen);
            }

            unsigned int decompLen = recvLen;
            if (useDecompress_) {
                // decompress data
                LOG_INFO(LOGTYPE_NETWORK, "decompress");
                decompLen = decompress_.huffmanDecompress(decompressedBuffer_ + decompressedIndex_, 0x10000 - decompressedIndex_, rawBuffer_, recvLen);
            } else {
                memcpy(decompressedBuffer_ + decompressedIndex_, rawBuffer_, recvLen);
            }

            LOG_INFO(LOGTYPE_NETWORK, "buffer dump:");
            dumpBuffer(decompressedBuffer_, decompLen);

            // parse packet

            // lock packet queue
            // enqueue packet
            // unlock packet queue

            // sleep a little
            usleep(10);

        } else if (recvLen == 0) { // peer has shut down
            LOG_INFO(LOGTYPE_NETWORK, "Peer has shut down socket");
            // set error indicator ?
            break;
        } else if (recvLen == -1 && running_) { // error
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            } else {
                #ifdef WIN32
                    LOGARG_ERROR(LOGTYPE_NETWORK, "Socket error in receiveRun(): %s", strerror(WSAGetLastError()));
                #else
                    LOGARG_ERROR(LOGTYPE_NETWORK, "Socket error in receiveRun(): %s", strerror(errno));
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
        printf("%x(%c) ", (uint8_t)buf[i], buf[i]);
    }
    printf("\n");
}

bool Socket::sendAll() {
    if (sendIndex_ == 0 || criticalError_) {
        return true;
    }

    LOG_INFO(LOGTYPE_NETWORK, "sendAll pre send:");
    dumpBuffer(sendBuffer_, sendIndex_);

    socketMutex_.lock();
    unsigned int sendLen = ::send(socketFd_, sendBuffer_, sendIndex_, 0);
    socketMutex_.unlock();

    LOG_INFO(LOGTYPE_NETWORK, "sendAll post send:");

    if (sendLen < 0) {
        #ifdef WIN32
            LOGARG_ERROR(LOGTYPE_NETWORK, "Socket error in sendAll(): %s", strerror(WSAGetLastError()));
        #else
            LOGARG_ERROR(LOGTYPE_NETWORK, "Socket error in sendAll(): %s", strerror(errno));
        #endif

        criticalError_ = true;

        return false;
    }

    if (sendLen != sendIndex_) {
        LOG_ERROR(LOGTYPE_NETWORK, "Socket error in sendAll(): Unable to send all bytes");
        return false;
    }

    sendIndex_ = 0;

    return true;
}

void Socket::writeSeed(uint32_t seed) {
    PacketWriter::write(sendBuffer_, 0x10000, sendIndex_, seed);
    LOGARG_DEBUG(LOGTYPE_NETWORK, "Index after seed: %u", sendIndex_);
}

void Socket::setUseDecompress(bool value) {
    useDecompress_ = value;
}

}
}
