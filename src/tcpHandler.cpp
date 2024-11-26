#include "tcpHandler.hpp"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdint>

void TcpHandler::InitSocket(int port)
{
	mPort = port;

  	mSockfd = socket(AF_INET, SOCK_STREAM, 0);
  	if (mSockfd < 0)
	{
		std::cerr << "socket: msockfd" << std::endl;
		exit(EXIT_FAILURE);
	}

  	// Bind socket
	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_addr.s_addr = INADDR_ANY;
	mServerAddr.sin_port = htons(mPort);
	if (bind(mSockfd, (struct sockaddr *)&mServerAddr, sizeof(mServerAddr)) < 0)
	{
		std::cerr << "bind error" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Listen for incoming connections
	listen(mSockfd, 5);
	std::cout << "[ port: " << mPort << " ] Waiting for a client to connect..." << std::endl;

	// Accept a connection from client
	mClientSock = accept(mSockfd, (struct sockaddr *)&mClientAddr, &mClientSockLen);
	if (mClientSock < 0)
	{
		std::cerr << "accept error" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "[ port: " << mPort << " ] Client connected" << std::endl;
}

void TcpHandler::SendData(const unsigned char* ci, size_t size)
{
    if (mClientSock < 0)
    {
        std::cerr << "[ port: " << mPort << " ] not valid client sock" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (send(mClientSock, ci, size, 0) < 0)
    {
		std::cerr << "[ port: " << mPort << " ] send data (size)" << std::endl;
	}
}

void TcpHandler::SendData(std::vector<uint8_t>& data)
{
    if (mClientSock < 0)
    {
        std::cerr << "[ port: " << mPort << " ] not valid client sock" << std::endl;
        exit(EXIT_FAILURE);
    }

	if (send(mClientSock, data.data(), data.size(), 0) < 0)
    {
        std::cerr << "[ port: " << mPort << " ] send data" << std::endl;
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
        std::cerr << "[ port: " << mPort << " ] not valid client sock" << std::endl;
        exit(EXIT_FAILURE);
    }

	if (send(mClientSock, json.dump().c_str(), json.dump().size(), 0) < 0)
	{
		std::cerr << "[ port: " << mPort << " ] send json" << std::endl;
	}
}