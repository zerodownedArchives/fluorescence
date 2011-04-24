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

    template<class P>
    bool write(const P& packet) {
        return packet.write(sendBuffer_, 0x10000, sendIndex_);
    }

    void writeSeed(uint32_t seed);

    bool sendAll();

private:
    int socketFd_;

    boost::mutex socketMutex_;

    int8_t rawBuffer_[0x4000];

    int8_t decompressedBuffer_[0x10000];
    unsigned int decompressedIndex_;

    int8_t sendBuffer_[0x10000];
    unsigned int sendIndex_;

    boost::shared_ptr<Encryption> encryption_;
    Decompress decompress_;

    bool running_;
    boost::thread receiveThread_;
    void receiveRun();


    void dumpBuffer(int8_t* buffer, unsigned int length);
};

}
}

#endif
