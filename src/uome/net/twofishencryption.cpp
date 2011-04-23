
#include "twofishencryption.hpp"

#include <misc/logger.hpp>

namespace uome {
namespace net {

TwofishEncryption::TwofishEncryption(uint32_t seed) {
    if (makeKey(&keyInstance_, DIR_ENCRYPT, 128, NULL) != TRUE || cipherInit(&cipherInstance_, MODE_ECB, NULL) != TRUE) {
        LOG_ERROR(LOGTYPE_NETWORK, "Error initializing twofish (makekey)");
        throw std::exception();
    }

    uint8_t* pKey = reinterpret_cast<uint8_t*>(&keyInstance_.key32);
    for (unsigned int i = 0; i < 4; i++) {
        *pKey++ = (uint8_t)((seed & 0xFF000000) >> 24);
        *pKey++ = (uint8_t)((seed & 0x00FF0000) >> 16);
        *pKey++ = (uint8_t)((seed & 0x0000FF00) >> 8);
        *pKey++ = (uint8_t)(seed & 0x000000FF);
    }

    LOGARG_DEBUG(LOGTYPE_NETWORK, "Twofish-Key: %X (*4)", keyInstance_.key32[0]);

    if (reKey(&keyInstance_) != TRUE) {
        LOG_ERROR(LOGTYPE_NETWORK, "Error initializing twofish (rekey)");
        throw std::exception();
    }

    for (unsigned int i = 0; i < TWOFISH_CRYPT_TABLE_LENGTH; i++) {
        internalBuffer_[i] = i;
    }

    reEncryptInternalBuffer();

    MD5_CTX md5;
    MD5Init(&md5);
    MD5Update(&md5, reinterpret_cast<unsigned char*>(internalBuffer_), TWOFISH_CRYPT_TABLE_LENGTH);
    MD5Final(reinterpret_cast<unsigned char*>(decryptTable_), &md5);

    receivePos_ = sendPos_ = 0;
}


void TwofishEncryption::encrypt(int8_t* dst, int8_t* src, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        if (sendPos_ >= TWOFISH_CRYPT_TABLE_LENGTH) {
            reEncryptInternalBuffer();
            sendPos_ = 0;
        }

        dst[i] = src[i] ^ internalBuffer_[sendPos_++];
    }
}

void TwofishEncryption::decrypt(int8_t* dst, int8_t* src, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        if (receivePos_ >= 0x10) {
            receivePos_ = 0;
        }

        dst[i] = src[i] ^ decryptTable_[receivePos_++];
    }
}

void TwofishEncryption::reEncryptInternalBuffer() {
    for (unsigned int i = 0; i < (TWOFISH_CRYPT_TABLE_LENGTH / 0x10); i++) {
        blockEncrypt(&cipherInstance_, &keyInstance_,
                     reinterpret_cast<unsigned char*>(&internalBuffer_[i * 0x10]), 128, reinterpret_cast<unsigned char*>(&internalBuffer_[i * 0x10]));
    }
}

}
}
