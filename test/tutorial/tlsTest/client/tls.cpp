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
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    createSSLContext();
}

void TLS::createSSLContext()
{
    const SSL_METHOD* method = TLS_client_method(); // 클라이언트용
    mCTX = SSL_CTX_new(method);
    if (!mCTX)
    {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

void TLS::PerformTLSHandshake(int clientFd)
{
    mSSL = SSL_new(mCTX);
    SSL_set_fd(mSSL, clientFd);

    if (SSL_connect(mSSL) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected with " << SSL_get_cipher(mSSL) << " encryption" << std::endl;
}

void TLS::CommunicateWithServer()
{
    const char* message = "Hello, TLS server!";
    SSL_write(mSSL, message, strlen(message));

    char buffer[1024] = {0};
    int bytesRead = SSL_read(mSSL, buffer, sizeof(buffer));
    if (bytesRead > 0) {
        std::cout << "Received: " << buffer << std::endl;
    }
}