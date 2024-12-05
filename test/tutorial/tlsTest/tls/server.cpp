#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

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

int createServerSocket(int port) {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        close(serverFd);
        exit(EXIT_FAILURE);
    }

    if (listen(serverFd, 1) < 0) {
        perror("Listen failed");
        close(serverFd);
        exit(EXIT_FAILURE);
    }

    return serverFd;
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
    initializeOpenSSL();
    SSL_CTX* ctx = createSSLContext();

    configureContext(ctx);

    int serverFd = createServerSocket(4433); // 예: 포트 4433

    std::cout << "Server is listening on port 4433..." << std::endl;

    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &clientLen);

        if (clientFd < 0) {
            perror("Accept failed");
            continue;
        }

        handleClientConnection(clientFd, ctx);
    }

    close(serverFd);
    SSL_CTX_free(ctx);
    cleanupOpenSSL();

    return 0;
}