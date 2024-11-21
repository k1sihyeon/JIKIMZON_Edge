#include "cipherHandler.hpp"

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <string.h>

CipherHandler::CipherHandler() : mCTX(nullptr) {}

CipherHandler::~CipherHandler()
{
    if (mCTX)
    {
        EVP_CIPHER_CTX_free(mCTX);
    }
}

unsigned char* CipherHandler::Init()
{
    OpenSSL_add_all_algorithms();

    mCTX = EVP_CIPHER_CTX_new();
    if (mCTX == nullptr)
    {
        std::cerr << "Error: new ctx" << std::endl;
    }

    if (!RAND_bytes(mKey, sizeof(mKey)))
    {
        std::cerr << "Error: generate key" << std::endl;
    }

    return mKey;
}

// unsigned char* CipherHandler::EncryptData(std::vector<uint8_t>& src, int size, uint8_t* dest, uint8_t* dedest)
unsigned char* CipherHandler::EncryptData(std::vector<uint8_t>& src, int size, uint8_t* dest)
{
    memset(mIV, 0, sizeof(mIV));
    if (!RAND_bytes(mIV, sizeof(mIV)))
    {
        std::cerr << "Error: generate iv" << std::endl;
    }

    if (EVP_EncryptInit_ex(mCTX, EVP_chacha20(), nullptr, mKey, mIV) != 1)
    {
        std::cerr << "Error: encrypt init" << std::endl;
    }

    int len;
    if (EVP_EncryptUpdate(mCTX, dest, &len, src.data(), size) != 1)
    {
        std::cerr << "Error: encrypt update" << std::endl;
    }

    // decrypt
    // if (EVP_DecryptInit_ex(mCTX, EVP_chacha20(), nullptr, mKey, mIV) != 1)
    // {
    //     std::cerr << "Error: encrypt init" << std::endl;
    // }

    // if (EVP_DecryptUpdate(mCTX, dedest, &len, dest, size) != 1)
    // {
    //     std::cerr << "Error: encrypt update" << std::endl;
    // }

    return mIV;
}

// bool CipherHandler::IsEqual(std::vector<uint8_t>& en, uint8_t* de, size_t size) {
//     for (size_t i = 0; i < size; i++) {
//         if (en[i] != de[i]) {
//             return false;
//         }
//     }
//     return true;
// }