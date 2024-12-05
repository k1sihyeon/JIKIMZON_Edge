#include "tls.hpp"

TLS::TLS()
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    createSSLContext();
    configureContext();
}

TLS::~TLS()
{
    EVP_cleanup();
}

void TLS::createSSLContext() {
    const SSL_METHOD* method = TLS_server_method();
    mCTX = SSL_CTX_new(method);
    if (!mCTX)
    {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

void TLS::configureContext() {
    // 인증서와 키 파일 경로를 설정하세요.
    if (SSL_CTX_use_certificate_file(mCTX, "server.crt", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(mCTX, "server.key", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}
