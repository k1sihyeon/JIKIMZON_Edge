#ifndef JIKIMZON_TLSHANDLER_H
#define JIKIMZON_TLSHANDLER_H

#include <openssl/ssl.h>
#include <nlohmann/json.hpp>
#include <vector>
#include "utils.hpp"

class TlsHandler {
public:
    TlsHandler(int clientFd);
    ~TlsHandler();

    // void SendData(const nlohmann::json& json);
    void SendData(std::vector<uint8_t>& data);

private:
    int mClientFd;
    SSL_CTX* mCTX;
    SSL* mSSL;
    Utils utils;

    void createSSLContext();
    void configureContext();
    void clientConnect();
};

#endif //JIKIMZON_TLSHANDLER_H