#include "cipherHandler.hpp"

int main()
{
    CipherHandler* cipherHandler = new CipherHandler();

    std::vector<uint8_t> plainText;
    std::vector<uint8_t> encryptedText;
    std::vector<uint8_t> decryptedText;
    unsigned char iv[12];

    // 암호화
    cipherHandler->EncryptData(iv, plainText, sizeof(plainText), encryptedText);
    cipherHandler->DecryptData(iv, encryptedText, sizeof(encryptedText), decryptedText);
    cipherHandler->Cmp(plainText,decryptedText);

    delete cipherHandler;
}