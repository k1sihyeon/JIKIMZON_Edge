#ifndef JIKIMZON_CIPHERHANDLER_H
#define JIKIMZON_CIPHERHANDLER_H

#include <vector>
#include <cstdint>
#include <openssl/evp.h>

class CipherHandler {
public:
    CipherHandler();
    ~CipherHandler();

    void EncryptData(unsigned char* iv, std::vector<uint8_t>& src, int size, std::vector<uint8_t>& dest);

private:
    unsigned char mKey[32];
    EVP_CIPHER_CTX* mCTX;
    void loadKey(const std::string& path);
};

#endif