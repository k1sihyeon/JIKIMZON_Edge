#ifndef JIKIMZON_TCPHANDLER_H
#define JIKIMZON_TCPHANDLER_H

#include <iostream>
#include <opencv2/opencv.hpp>

#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT    12345

class TcpHandler {
public:
    TcpHandler() = default;
    ~TcpHandler() = default;
    
    void InitSocket();
    void SendFrame(const unsigned char* ci, size_t size);
    void SendFrame(const uint8_t* frame, size_t size);
    

private:
    
    static TcpHandler* sInstance;

    int mSockfd;
    int mClientSock;
    struct sockaddr_in mServerAddr;
    struct sockaddr_in mClientAddr;
    socklen_t mClientSockLen = sizeof(mClientAddr);
};

#endif