#ifndef JIKIMZON_TCPHANDLER_H
#define JIKIMZON_TCPHANDLER_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "frame.hpp"

class TcpHandler
{
public:
    TcpHandler() = default;
    ~TcpHandler() = default;
    
    int InitSocket(int port);

private:
    int mSockfd;
    int mClientSock;
    int mPort;
    struct sockaddr_in mServerAddr;
    struct sockaddr_in mClientAddr;
    socklen_t mClientSockLen = sizeof(mClientAddr);
};

#endif