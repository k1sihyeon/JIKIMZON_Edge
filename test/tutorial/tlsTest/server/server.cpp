#include "tls.hpp"
#include "tcp.hpp"

void initializeOpenSSL() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void cleanupOpenSSL() {
    EVP_cleanup();
}

SSL_CTX* createSSLContext() {
    const SSL_METHOD* method = TLS_server_method(); // 서버의 경우
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

void configureContext(SSL_CTX* ctx) {
    // 인증서와 키 파일 경로를 설정하세요.
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

void handleClientConnection(int clientFd, SSL_CTX* ctx) {
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, clientFd);

    if (SSL_accept(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    } else {
        std::cout << "TLS handshake successful!" << std::endl;

        const char* reply = "Hello, TLS client!";
        SSL_write(ssl, reply, strlen(reply));
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(clientFd);
}

int main() {
    TLS* tls = new TLS();
    TCP* tcp = new TCP();

    tls->Init();
    int serverFd = tcp->CreateServerSocket();

    std::cout << "Server is listening on port 4433..." << std::endl;

    int clientFd = tcp->AcceptClient();
    tls->HandleClientConnection(clientFd);

    return 0;
}