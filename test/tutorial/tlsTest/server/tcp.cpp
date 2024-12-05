#include "tcp.hpp"

TCP::TCP()
{
    mPort = 4433;
}

TCP::~TCP()
{
    if (mServerFd)
    {
        close(mServerFd);
    }

    if (mClientFd)
    {
        close(mServerFd);
    }
}

int TCP::CreateServerSocket() {
    mServerFd = socket(AF_INET, SOCK_STREAM, 0);
    if (mServerFd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(mPort);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(mServerFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(mServerFd);
        exit(EXIT_FAILURE);
    }

    if (listen(mServerFd, 1) < 0) {
        perror("Listen failed");
        close(mServerFd);
        exit(EXIT_FAILURE);
    }

    return mServerFd;
}

int TCP::AcceptClient()
{
    socklen_t clientLen = sizeof(clientAddr);
    mClientFd = accept(mServerFd, (struct sockaddr*)&clientAddr, &clientLen);

    if (mClientFd < 0)
    {
        perror("Accept failed");
    }

    return mClientFd;
}