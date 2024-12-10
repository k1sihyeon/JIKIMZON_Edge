#include "cipherHandler.hpp"
#include "utils.hpp"

#include <iostream>
#include <cstring>
#include <fstream>

CipherHandler::CipherHandler()
{
    mCTX = EVP_CIPHER_CTX_new();
    if (mCTX == nullptr)
    {
        std::cerr << "Error: new ctx" << std::endl;
    }

    Utils utils; 
    loadKey(utils.GetWorkingDir() + "/src/keyfile.bin");
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


std::vector<uint8_t> CipherHandler::EncryptData(std::string timestamp, std::vector<uint8_t> src, int size)
{   
    std::vector<uint8_t> dest;
    dest.resize(size);

    unsigned char iv[12];
    std::memcpy(iv, reinterpret_cast<const unsigned char*>(timestamp.substr(timestamp.length() - 12, 12).c_str()), 12);

    if (EVP_EncryptInit_ex(mCTX, EVP_chacha20(), nullptr, mKey, iv) != 1)
    {
        std::cerr << "Error: encrypt init" << std::endl;
    }

    int len;
    if (EVP_EncryptUpdate(mCTX, dest.data(), &len, src.data(), size) != 1)
    {
        std::cerr << "Error: encrypt update" << std::endl;
    }

    return dest;
}