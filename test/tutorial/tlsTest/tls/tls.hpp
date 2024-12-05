#ifndef JIKIMZON_TLS_H
#define JIKIMZON_TLS_H

#include <openssl/ssl.h>
#include <openssl/err.h>

class TLS {
public:
    TLS();
    ~TLS();
    
private:
    SSL_CTX* mCTX;
    void createSSLContext();
    void configureContext();
};

#endif