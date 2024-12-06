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
    
    void InitSocket(int port);
    void SendData(const unsigned char* ci, size_t size);
    void SendData(std::vector<uint8_t>& data);
    void SendFrame(frame::Frame& frame);
    void SendJson(const nlohmann::json& json);

private:
    int mSockfd;
    int mClientSock;
    int mPort;
    struct sockaddr_in mServerAddr;
    struct sockaddr_in mClientAddr;
    socklen_t mClientSockLen = sizeof(mClientAddr);
};

#endif