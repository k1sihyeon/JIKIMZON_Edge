#ifndef JIKIMZON_CIPHERHANDLER_H
#define JIKIMZON_CIPHERHANDLER_H

#include <vector>
#include <cstdint>
#include <openssl/evp.h>
#include <openssl/rand.h>

class CipherHandler {
public:
    CipherHandler(int sock);
    ~CipherHandler();

    void EncryptData(std::vector<uint8_t>& src, int size, uint8_t* dest, uint8_t* dedest);
    void SendEncryptedData(int size, uint8_t* data);

    // bool IsEqual(std::vector<uint8_t>& en, uint8_t* de, size_t size);

private:
    int mSock;
    unsigned char mKey[32];
    unsigned char mIV[12];

    EVP_CIPHER_CTX* mCTX;

    void init();
};

#endif