#include "tcpHandler.hpp"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdint>

int TcpHandler::InitSocket(int port)
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
	listen(mSockfd, 1);
	std::cout << "[ port: " << mPort << " ] Waiting for a client to connect..." << std::endl;

	// Accept a connection from client
	mClientSock = accept(mSockfd, (struct sockaddr *)&mClientAddr, &mClientSockLen);
	if (mClientSock < 0)
	{
		std::cerr << "accept error" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "[ port: " << mPort << " ] Client connected" << std::endl;

	return mClientSock;
}