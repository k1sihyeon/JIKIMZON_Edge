#include "cipherHandler.hpp"

#include <iostream>

void print(std::vector<uint8_t>& src, size_t size);
void cmp(std::vector<uint8_t>& src, size_t size, std::vector<uint8_t>& dest);

int main()
{
    CipherHandler* cipherHandler = new CipherHandler();

    std::vector<uint8_t> plainText = {1, 2, 3, 4, 5};
    std::vector<uint8_t> encryptedText(plainText.size(), 0);
    std::vector<uint8_t> decryptedText(plainText.size(), 0);
    unsigned char iv[12];

    std::cout << "Plain" << std::endl;
    print(plainText, plainText.size());

    std::cout << "Encrypt" << std::endl;
    cipherHandler->EncryptData(iv, plainText, plainText.size(), encryptedText);
    print(encryptedText, encryptedText.size());

    std::cout << "Decrypt" << std::endl;
    cipherHandler->DecryptData(iv, encryptedText, encryptedText.size(), decryptedText);
    print(decryptedText, decryptedText.size());

    cmp(plainText, plainText.size(), decryptedText);

    delete cipherHandler;
}

void print(std::vector<uint8_t>& src, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        std::cout << +src[i] << " ";
    }
    std::cout << std::endl;
}

void cmp(std::vector<uint8_t>& src, size_t size, std::vector<uint8_t>& dest)
{
    for (size_t i = 0; i < size; i++)
    {
        if (src[i] != dest[i])
        {
            std::cout << "FAILED" << std::endl;
            return;
        }
    }
    std:: cout << "SUCCESSED" << std::endl;
    return;
}