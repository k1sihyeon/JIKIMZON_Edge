#include "tls.hpp"

TLS::~TLS()
{   
    SSL_shutdown(mSSL);
    SSL_free(mSSL);

    SSL_CTX_free(mCTX);
    EVP_cleanup();
}

void TLS::Init()
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    createSSLContext();
    configureContext();
}

void TLS::createSSLContext()
{
    const SSL_METHOD* method = TLS_server_method();
    mCTX = SSL_CTX_new(method);
    if (!mCTX)
    {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

void TLS::configureContext()
{
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

void TLS::HandleClientConnection(int clientFd)
{
    mSSL = SSL_new(mCTX);
    SSL_set_fd(mSSL, clientFd);

    if (SSL_accept(mSSL) <= 0)
    {
        ERR_print_errors_fp(stderr);
    }
    else
    {
        std::cout << "TLS handshake successful!" << std::endl;

        const char* reply = "Hello, TLS client!";
        SSL_write(mSSL, reply, strlen(reply));
    }
}