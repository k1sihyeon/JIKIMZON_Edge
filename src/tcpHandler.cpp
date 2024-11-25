#include "tcpHandler.hpp"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdint>

void TcpHandler::InitSocket()
{
  	mSockfd = socket(AF_INET, SOCK_STREAM, 0);
  	if (mSockfd < 0)
	{
		std::cerr << "socket: msockfd" << std::endl;
		exit(EXIT_FAILURE);
	}

  	// Bind socket
	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_addr.s_addr = INADDR_ANY;
	mServerAddr.sin_port = htons(PORT);
	if (bind(mSockfd, (struct sockaddr *)&mServerAddr, sizeof(mServerAddr)) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	// Listen for incoming connections
	listen(mSockfd, 5);
	printf("Waiting for a client to connect...\n");

	// Accept a connection from client
	mClientSock = accept(mSockfd, (struct sockaddr *)&mClientAddr, &mClientSockLen);
	if (mClientSock < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	printf("Client connected!\n");
}

void TcpHandler::SendData(const unsigned char* ci, size_t size)
{
    if (mClientSock < 0)
    {
        std::cerr << "not valid client sock" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (send(mClientSock, ci, size, 0) < 0)
    {
        perror("send iv");
	}
}

void TcpHandler::SendData(std::vector<uint8_t>& data)
{
    if (mClientSock < 0)
    {
        std::cerr << "not valid client sock" << std::endl;
        exit(EXIT_FAILURE);
    }

	if (send(mClientSock, data.data(), data.size(), 0) < 0)
    {
        perror("send data");
    }
}

void TcpHandler::SendFrame(frame::Frame& frame)
{
	std::vector<uint8_t> buffer = frame.Serialize();
	this->SendData(buffer);
}

void TcpHandler::SendJson(const nlohmann::json& json)
{
	if (mClientSock < 0)
    {
        std::cerr << "not valid client sock" << std::endl;
        exit(EXIT_FAILURE);
    }

	if (send(mClientSock, json.dump().c_str(), json.dump().size(), 0) < 0)
	{
		perror("send json");
	}
}