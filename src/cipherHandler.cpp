#include "cipherHandler.hpp"

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <string.h>

CipherHandler::CipherHandler(int sock) : mSock(sock), mCTX(nullptr)
{
    init();
}

CipherHandler::~CipherHandler()
{
    if (mCTX)
    {
        EVP_CIPHER_CTX_free(mCTX);
    }
}

void CipherHandler::init()
{
    mCTX = EVP_CIPHER_CTX_new();
    if (mCTX != 0)
    {
        std::cerr << "Error: new ctx" << std::endl;
    }

    if (RAND_bytes(mKey, sizeof(mKey)))
    {
        std::cerr << "Error: generate key" << std::endl;
    }

    send(mSock, mKey, sizeof(mKey), 0);
}

void CipherHandler::EncryptData(uint8_t* src, int size, uint8_t* dest)
{
    memset(mIV, 0, sizeof(mIV));
    if (RAND_bytes(mIV, sizeof(mIV)))
    {
        std::cerr << "Error: generate iv" << std::endl;
    }

    if (EVP_EncryptInit_ex(mCTX, EVP_chacha20(), nullptr, mKey, mIV) != 1)
    {
        std::cerr << "Error: encrypt init" << std::endl;
    }

    int len;
    if (EVP_EncryptUpdate(mCTX, dest, &len, src, size) != 1)
    {
        std::cerr << "Error: encrypt update" << std::endl;
    }
}

void CipherHandler::SendEncryptedData(int size, uint8_t* data)
{
    send(mSock, mIV, sizeof(mIV), 0);
    send(mSock, data, size, 0);
}

void CipherHandler::decryptData(uint8_t* src, int size, uint8_t* dest)
{
    memset(mIV, 0, sizeof(mIV));
    if (RAND_bytes(mIV, sizeof(mIV)))
    {
        std::cerr << "Error: generate iv" << std::endl;
    }

    if (EVP_EncryptInit_ex(mCTX, EVP_chacha20(), nullptr, mKey, mIV) != 1)
    {
        std::cerr << "Error: encrypt init" << std::endl;
    }

    int len;
    if (EVP_DecryptUpdate(mCTX, dest, &len, src, size) != 1)
    {
        std::cerr << "Error: encrypt update" << std::endl;
    }
}