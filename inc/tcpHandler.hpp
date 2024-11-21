#ifndef JIKIMZON_TCPHANDLER_H
#define JIKIMZON_TCPHANDLER_H

#include <iostream>
#include <opencv2/opencv.hpp>

#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT    12345

class TcpHandler
{
public:
    TcpHandler() = default;
    ~TcpHandler() = default;
    
    void InitSocket();
    void SendData(const unsigned char* ci, size_t size);
    void SendData(std::vector<uint8_t>& frame);
    

private:
    
    int mSockfd;
    int mClientSock;
    struct sockaddr_in mServerAddr;
    struct sockaddr_in mClientAddr;
    socklen_t mClientSockLen = sizeof(mClientAddr);
};

#endif