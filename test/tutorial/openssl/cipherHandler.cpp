#include "cipherHandler.hpp"

#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>

CipherHandler::CipherHandler()
{
    mCTX = EVP_CIPHER_CTX_new();
    if (mCTX == nullptr)
    {
        std::cerr << "Error: new ctx" << std::endl;
    }

    loadKey("./keyfile.bin");
}

CipherHandler::~CipherHandler()
{
    if (mCTX)
    {
        EVP_CIPHER_CTX_free(mCTX);
    }
}

void CipherHandler::loadKey(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: cipher - file open" << std::endl;
    }

    std::vector<uint8_t> vec((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    if (vec.size() != 32)
    {
        std::cerr << "Error: generate key failed" << std::endl;
    }

    auto key = reinterpret_cast<unsigned char*>(vec.data());
    memcpy(mKey, key, 32);
}

void CipherHandler::EncryptData(unsigned char* iv, std::vector<uint8_t>& src, int size, std::vector<uint8_t>& dest)
{
    for (unsigned char i = 0; i < 12; i++)
    {
        iv[i] = 0x02;
    }
    
    // TODO: set iv

    if (EVP_EncryptInit_ex(mCTX, EVP_chacha20(), nullptr, mKey, iv) != 1)
    {
        std::cerr << "Error: encrypt init" << std::endl;
    }

    int len;
    if (EVP_EncryptUpdate(mCTX, dest.data(), &len, src.data(), size) != 1)
    {
        std::cerr << "Error: encrypt update" << std::endl;
    }
}

void CipherHandler::DecryptData(unsigned char* iv, std::vector<uint8_t>& src, int size, std::vector<uint8_t>& dest)
{
    if (EVP_DecryptInit_ex(mCTX, EVP_chacha20(), nullptr, mKey, iv) != 1)
    {
        std::cerr << "Error: decrypt init" << std::endl;
    }

    int len;
    if (EVP_DecryptUpdate(mCTX, dest.data(), &len, src.data(), size) != 1)
    {
        std::cerr << "Error: decrypt update" << std::endl;
    }
}