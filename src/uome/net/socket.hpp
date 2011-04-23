#ifndef UOME_NET_SOCKET_HPP
#define UOME_NET_SOCKET_CPP

#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "encryption.hpp"
#include "decompress.hpp"

namespace uome {
namespace net {

class Socket {
public:
    Socket();
    ~Socket();

    void setEncryption(boost::shared_ptr<Encryption> value);

    bool connect(const std::string& host, unsigned short port);
    void close();

private:
    int socketFd_;

    boost::mutex socketMutex_;

    int8_t rawBuffer_[0x4000];

    int8_t decryptedBuffer_[0x4000];
    unsigned int decryptedIndex_;

    int8_t decompressedBuffer_[0x10000];
    unsigned int decompressedIndex_;

    int8_t sendBuffer[0x10000];
    unsigned int sendIndex_;

    boost::shared_ptr<Encryption> encryption_;
    Decompress decompress_;

    bool running_;
    boost::thread receiveThread_;
    void receiveRun();
};

}
}

#endif
