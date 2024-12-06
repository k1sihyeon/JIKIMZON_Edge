#include "tlsHandler.hpp"

#include <openssl/err.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

TlsHandler::TlsHandler(int clientFd): mClientFd(clientFd)
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    createSSLContext();
    configureContext();
    clientConnect();
}

TlsHandler::~TlsHandler()
{
    EVP_cleanup();
    if (mSSL)
    {
        SSL_shutdown(mSSL);
        SSL_free(mSSL);
    }
}

void TlsHandler::createSSLContext()
{
    const SSL_METHOD* method = TLS_server_method();
    mCTX = SSL_CTX_new(method);
    if (!mCTX)
    {
        std::cerr << "Failed create ssl context" << std::endl;
    }
}

void TlsHandler::configureContext()
{
    if (SSL_CTX_use_certificate_file(mCTX, "../certs/server.crt", SSL_FILETYPE_PEM) <= 0)
    {
        std::cerr << "Failed get certificate" << std::endl;
    }

    if (SSL_CTX_use_PrivateKey_file(mCTX, "../certs/server.key", SSL_FILETYPE_PEM) <= 0)
    {
        std::cerr << "Failed get private key" << std::endl;
    }
}

void TlsHandler::clientConnect() {
    mSSL = SSL_new(mCTX);
    SSL_set_fd(mSSL, mClientFd);

    if (SSL_accept(mSSL) <= 0)
    {
        std::cerr << "Failed ssl accept" << std::endl;
    }
}

void TlsHandler::SendData(const nlohmann::json& json)
{
    SSL_write(mSSL, json.dump().c_str(), json.dump().size());
}

void TlsHandler::SendData(std::vector<uint8_t>& data)
{
    SSL_write(mSSL, data.data(), data.size());
}