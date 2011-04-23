#ifndef UOME_NET_TWOFISHENCRYPTION_HPP
#define UOME_NET_TWOFISHENCRYPTION_HPP

#include "encryption.hpp"

#include "twofish/aes.h"
#include "md5/md5.h"

namespace uome {
namespace net {

const unsigned int TWOFISH_CRYPT_TABLE_LENGTH = 256;

class TwofishEncryption : public Encryption {
public:
    TwofishEncryption(unsigned int seed);

    virtual void encrypt(int8_t* dst, int8_t* src, unsigned int len);
    virtual void decrypt(int8_t* dst, int8_t* src, unsigned int len);

private:
    cipherInstance cipherInstance_;
    keyInstance keyInstance_;

    int8_t internalBuffer_[TWOFISH_CRYPT_TABLE_LENGTH];
    int8_t decryptTable_[16];

    unsigned int receivePos_;
    unsigned int sendPos_;

    void reEncryptInternalBuffer();
};

}
}

#endif
