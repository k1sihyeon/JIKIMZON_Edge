#include "cipherHandler.hpp"
#include "tcpHandler.hpp"

#include <iostream>

CipherHandler::CipherHandler() : mCTX(nullptr)
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

    send(TcpHandler::GetSock(), mKey, sizeof(mKey), 0);
}

void CipherHandler::EncryptData(const unsigned char* data, int dataLen,
                const unsigned char* key, const unsigned char* iv,
                unsigned char* ciphered)
{
    memset(mIV, 0, sizeof(mIV));
    if (RAND_bytes(mIV, sizeof(mIV)))
    {
        std::cerr << "Error: generate iv" << std::endl;
    }

    if (EVP_EncryptInit_ex(mCTX, EVP_chacha20(), nullptr, key, iv) != 1)
    {
        std::cerr << "Error: encrypt init" << std::endl;
    }

    int len;
    if (EVP_EncryptUpdate(mCTX, ciphered, &len, data, dataLen) != 1)
    {
        std::cerr << "Error: encrypt update" << std::endl;
    }
}

void CipherHandler::SendEncryptedData(int dataLen, unsigned char* ciphered)
{
    send(TcpHandler::GetSock, mIV, sizeof(mIV), 0);
    send(TcpHandler::GetSock, ciphered, , 0);
}