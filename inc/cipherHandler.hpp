#ifndef JIKIMZON_CIPHERHANDLER_H
#define JIKIMZON_CIPHERHANDLER_H

#include <openssl/evp.h>
#include <openssl/rand.h>

class CipherHandler {
public:
    CipherHandler();
    ~CipherHandler();

    void EncryptData(const unsigned char* data, int dataLen, unsigned char* ciphered);
    void SendEncryptedData(int dataLen, unsigned char* ciphered);
    
private:
    // const char* server_ip;
    unsigned char mKey[32];
    unsigned char mIV[12];

    EVP_CIPHER_CTX* mCTX;

    void init();
};

#endif