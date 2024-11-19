#ifndef JIKIMZON_CIPHERHANDLER_H
#define JIKIMZON_CIPHERHANDLER_H

#include <openssl/evp.h>
#include <openssl/rand.h>

class CipherHandler {
public:
    CipherHandler(int sock);
    ~CipherHandler();

    void EncryptData(const std::vector<uint8_t>& frame, uint8_t* ciphered);
    void SendEncryptedData(int dataLen, uint8_t* ciphered);
    
private:
    int mSock;
    unsigned char mKey[32];
    unsigned char mIV[12];

    EVP_CIPHER_CTX* mCTX;

    void init();
};

#endif