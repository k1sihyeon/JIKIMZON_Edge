#ifndef JIKIMZON_CIPHERHANDLER_H
#define JIKIMZON_CIPHERHANDLER_H

#include <vector>
#include <cstdint>
#include <openssl/evp.h>
#include <openssl/rand.h>

class CipherHandler {
public:
    CipherHandler();
    ~CipherHandler();

    unsigned char* Init();

    // unsigned char* EncryptData(std::vector<uint8_t>& src, int size, uint8_t* dest, std::vector<uint8_t>& dedest);
    unsigned char* EncryptData(std::vector<uint8_t>& src, int size, std::vector<uint8_t>& dest);

    // bool IsEqual(std::vector<uint8_t>& en, uint8_t* de, size_t size);

private:
    unsigned char mKey[32];

    EVP_CIPHER_CTX* mCTX;
};

#endif