#ifndef JIKIMZON_TLS_H
#define JIKIMZON_TLS_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

class TLS {
public:
    TLS() = default;
    ~TLS();

    void Init();
    void PerformTLSHandshake(int clientFd);
    void CommunicateWithServer();

private:
    SSL_CTX* mCTX;
    SSL* mSSL;

    void createSSLContext();
    void configureContext();
};
    
#endif