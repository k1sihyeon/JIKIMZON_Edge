#ifndef JIKIMZON_CIPHERHANDLER_H
#define JIKIMZON_CIPHERHANDLER_H

#include <string>
#include <vector>
#include <cstdint>
#include <openssl/evp.h>

#define OUT

class CipherHandler {
public:
    CipherHandler();
    ~CipherHandler();

    void EncryptData(std::string& timestamp, std::vector<uint8_t>& src, int size, std::vector<uint8_t>& OUT dest);

private:
    unsigned char mKey[32];
    EVP_CIPHER_CTX* mCTX;
    void loadKey(const std::string& path);
};

#endif