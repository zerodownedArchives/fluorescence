
#include "socket.hpp"

#include <misc/logger.hpp>

namespace uome {
namespace net {

Socket::Socket() : socketFd_(0), decryptedIndex_(0), sendIndex_(0), running_(false) {
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

    sendIndex_ = 0;
    decryptedIndex_ = 0;
    running_ = true;

    receiveThread_ = boost::thread(&Socket::receiveRun, this);

    return true;
}

void Socket::close() {
    if (socketFd_) {
        LOG_INFO(LOGTYPE_NETWORK, "Closing socket");
        ::close(socketFd_);
        socketFd_ = 0;
    }

    running_ = false;
}

void Socket::receiveRun() {
    while (running_) {
        // lock socket mutex
        // receive bytes
        // unlock socket mutex

        // decrypt data

        // decompress data

        // parse packet

        // lock packet queue
        // enqueue packet
        // unlock packet queue

        // sleep a little
    }
}

}
}
