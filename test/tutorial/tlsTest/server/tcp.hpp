#ifndef JIKIMZON_TCP_H
#define JIKIMZON_TCP_H

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

class TCP {
public:
    TCP();
    ~TCP();

    int CreateServerSocket();
    int AcceptClient();

private:
    int mPort;
    int mServerFd;
    int mClientFd;
    sockaddr_in serverAddr;
    sockaddr_in clientAddr;
};

#endif