#ifndef UOME_NET_SOCKET_HPP
#define UOME_NET_SOCKET_HPP

#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "encryption.hpp"
#include "decompress.hpp"
#include "packet.hpp"

namespace uome {
namespace net {

class Socket {
public:
    Socket();
    ~Socket();

    void setEncryption(boost::shared_ptr<Encryption> value);

    bool connect(const std::string& host, unsigned short port);
    void close();

    bool isConnected();

    template<class P>
    bool write(const P& packet) {
        return packet.write(sendBuffer_, 0x10000, sendSize_);
    }

    void writeSeed(uint32_t seed);

    bool sendAll();

    void setUseDecompress(bool value);

private:
    int socketFd_;

    boost::mutex socketMutex_;

    int8_t rawBuffer_[0x4000];

    int8_t decompressedBuffer_[0x10000];
    unsigned int decompressedSize_;

    int8_t sendBuffer_[0x10000];
    unsigned int sendSize_;

    boost::shared_ptr<Encryption> encryption_;

    Decompress decompress_;
    bool useDecompress_;

    bool running_;
    boost::thread receiveThread_;
    void receiveRun();

    bool criticalError_;

    void dumpBuffer(int8_t* buffer, unsigned int length);


    boost::mutex packetQueueMutex_;
    std::list<boost::shared_ptr<Packet> > packetQueue_;
    void parsePackets();
};

}
}

#endif
