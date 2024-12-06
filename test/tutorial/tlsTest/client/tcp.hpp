#ifndef JIKIMZON_TCP_H
#define JIKIMZON_TCP_H

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

class TCP {
public:
    TCP();
    ~TCP();

    int CreateClientSocket();

private:
    int mPort;
    int mClientFd;
    char* mHostname;
    sockaddr_in mServerAddr;
};

#endif