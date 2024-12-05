#include "tcp.hpp"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

TCP::TCP()
{
    mPort = 8080;
    createServerSocket();
}

TCP::~TCP()
{
    if (mSocket)
    {
        close(mSocket);
    }
}

void TCP::createServerSocket() {
    mSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocket < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(mPort);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(mSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("Bind failed");
        close(mSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(mSocket, 1) < 0)
    {
        perror("Listen failed");
        close(mSocket);
        exit(EXIT_FAILURE);
    }
}