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



#include "twofishencryption.hpp"

#include <string.h>

#include <misc/log.hpp>

namespace fluo {
namespace net {

TwofishEncryption::TwofishEncryption(uint32_t seed) {
    memset(&keyInstance_, 0, sizeof(keyInstance));
    memset(&cipherInstance_, 0, sizeof(cipherInstance));

    if (makeKey(&keyInstance_, DIR_ENCRYPT, 128, NULL) != TRUE || cipherInit(&cipherInstance_, MODE_ECB, NULL) != TRUE) {
        LOG_EMERGENCY << "Error initializing twofish (makekey)" << std::endl;
        throw std::exception();
    }

    keyInstance_.key32[0] = keyInstance_.key32[1] = keyInstance_.key32[2] = keyInstance_.key32[3] = seed;
    //uint8_t* pKey = reinterpret_cast<uint8_t*>(&keyInstance_.key32);
    //for (unsigned int i = 0; i < 4; i++) {
        //*pKey++ = (uint8_t)((seed & 0xFF000000) >> 24);
        //*pKey++ = (uint8_t)((seed & 0x00FF0000) >> 16);
        //*pKey++ = (uint8_t)((seed & 0x0000FF00) >> 8);
        //*pKey++ = (uint8_t)(seed & 0x000000FF);
    //}

    LOG_DEBUG << "Twofish-Key: " << std::hex << keyInstance_.key32[0] << std::dec << std::endl;

    if (reKey(&keyInstance_) != TRUE) {
        LOG_ERROR << "Error initializing twofish (rekey)" << std::endl;
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


void TwofishEncryption::encrypt(int8_t* dst, const int8_t* src, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        if (sendPos_ >= TWOFISH_CRYPT_TABLE_LENGTH) {
            reEncryptInternalBuffer();
            sendPos_ = 0;
        }

        dst[i] = src[i] ^ internalBuffer_[sendPos_++];
    }
}

void TwofishEncryption::decrypt(int8_t* dst, const int8_t* src, unsigned int len) {
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
