#ifndef JIKIMZON_TCPHANDLER_H
#define JIKIMZON_TCPHANDLER_H

#include <iostream>
#include <opencv2/opencv.hpp>

#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT    12345

class TcpHandler {
public:
    TcpHandler();
    ~TcpHandler();

    void InitSocket();
    void SendFrame(cv::Mat frame);

    static TcpHandler* GetInstance()
    {
        if (instance == NULL)
        {
            instance = new TcpHandler();
        }
        return instance;
    }

private:
    static TcpHandler* instance;

    int sockfd;
    int clientSock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

};

#endif