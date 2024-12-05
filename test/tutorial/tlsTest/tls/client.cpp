#include <openssl/ssl.h>
#include <openssl/err.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

SSL_CTX* createSSLContext() {
    const SSL_METHOD* method = TLS_client_method(); // 클라이언트용
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

int createClientSocket(const char* hostname, int port) {
    int clientFd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientFd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, hostname, &serverAddr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        close(clientFd);
        exit(EXIT_FAILURE);
    }

    if (connect(clientFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(clientFd);
        exit(EXIT_FAILURE);
    }

    return clientFd;
}

void performTLSHandshake(SSL* ssl) {
    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected with " << SSL_get_cipher(ssl) << " encryption" << std::endl;
}

void communicateWithServer(SSL* ssl) {
    const char* message = "Hello, TLS server!";
    SSL_write(ssl, message, strlen(message));

    char buffer[1024] = {0};
    int bytesRead = SSL_read(ssl, buffer, sizeof(buffer));
    if (bytesRead > 0) {
        std::cout << "Received: " << buffer << std::endl;
    }
}

int main() {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    SSL_CTX* ctx = createSSLContext();
    int clientFd = createClientSocket("127.0.0.1", 4433); // 서버 주소와 포트

    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, clientFd);

    performTLSHandshake(ssl);
    communicateWithServer(ssl);

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(clientFd);

    SSL_CTX_free(ctx);
    EVP_cleanup();

    return 0;
}