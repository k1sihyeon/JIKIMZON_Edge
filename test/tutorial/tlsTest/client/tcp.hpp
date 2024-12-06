#ifndef JIKIMZON_TCP_H
#define JIKIMZON_TCP_H

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>

class TCP {
public:
    TCP();
    ~TCP();

    int CreateClientSocket();

private:
    int mPort;
    int mSocketFd;
    std::string mHostname;
};

#endif